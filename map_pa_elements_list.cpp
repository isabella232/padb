#include "map_pa_elements_list.h"
#include "qgsmaplayerregistry.h"
#include "qgsvectorlayer.h"
#include "qgsvectordataprovider.h"
#include "qgsgeometry.h"
#include "qgisinterface.h"
#include "qgsmapcanvas.h"
#include "qgsmaptopixel.h"
#include "qgsmaprenderer.h"
#include "qgsfield.h"
#include "pa_manager.h"
#include "resources.h"
#include "settings.h"
#include <algorithm>
#include <QMessageBox>

pa_db::map_pa_elements_list::map_pa_elements_list( pa_db::pa_manager *manager )
	: QObject( manager )
	, m_manager( manager )
{
}

void pa_db::map_pa_elements_list::reload( )
{
	load_free_elements( );
}

QString feature_name( QgsFeature &f, int field_key, const QString &layer_name )
{
	QString result;
	if ( field_key != -1 )
	{
		if ( f.attributeMap( ).count( field_key ) != 0 )
		{
			const QVariant var_name = f.attributeMap( ).value( field_key );
			if ( var_name.isValid( ) && !var_name.isNull( ) )
				result = var_name.toString( );
		}
	}
	if ( result.isEmpty( ) )
	{
		result += layer_name + " - ";
		if ( f.geometry( ) )
		{
			switch ( f.geometry( )->type( ) )
			{
			default:
				Q_ASSERT( false );
			case QGis::UnknownGeometry:
				result += "Unknown";
				break;
			case QGis::Polygon:
				result += "Polygon";
				break;
			case QGis::Line:
				result += "Line";
				break;
			case QGis::Point:
				result += "Point";
				break;
			}
		}
		else
		{
			result += "Unknown";
		}
	}
	return result;
}

void pa_db::map_pa_elements_list::load_free_elements( )
{
	clear( );
	bool found( false );
	QMap< QString, QgsMapLayer* > layers = QgsMapLayerRegistry::instance( )->mapLayers( );
	for ( QMap< QString, QgsMapLayer* >::iterator it_layer = layers.begin( );
		it_layer != layers.end( ); ++it_layer )
	{
		QgsMapLayer* lay = it_layer.value( );
		if ( QgsVectorLayer *cur_layrer = qobject_cast< QgsVectorLayer* >( it_layer.value( ) ) )
		{
			if ( cur_layrer->name( ) == settings::ooptLayer( ) )
			{
				found = true;
				if ( QgsVectorDataProvider *data_provider = cur_layrer->dataProvider( ) )
				{
					QgsAttributeList all = data_provider->attributeIndexes( );
					data_provider->select( all, QgsRectangle( ), false );

					const QMap< int, QgsField > data_fields = data_provider->fields( );
					int name_field_key = -1;
					for ( QMap< int, QgsField >::const_iterator it_fields = data_fields.constBegin( );
						it_fields != data_fields.constEnd( ); it_fields++ )
						if ( it_fields.value( ).name( ).toLower( ) == "name" )
						{
							name_field_key = it_fields.key( );
							break;
						}
					QgsFeature next_feature;
					while ( data_provider->nextFeature( next_feature ) )
					{
						pa_element element;
						element.set_layer_id( cur_layrer->name( ) );
						element.set_feature_id( next_feature.id( ) );
						element.set_feature_type( feature_name( next_feature, name_field_key, cur_layrer->name( ) ) );
						push_back( element );
					}
				}
			}
		}
	}
	if ( !found )
	{
		QMessageBox::information( 0, pa_db::text::pa_db_plugin_name( ), pa_db::text::message_no_layer( ) );
	}
}

QVector< pa_db::pa_element > pa_db::map_pa_elements_list::free_elements( ) const
{
	QVector< pa_element > result;
	for ( int nn = 0; nn < count( ); ++nn )
		if ( m_manager->is_element_free( at( nn ) ) )
			result.push_back( at( nn ) );
	return result;
}

void pa_db::map_pa_elements_list::select( const pa_element &el, bool zoom )
{
	QMap< QString, QgsMapLayer* > layers = QgsMapLayerRegistry::instance( )->mapLayers( );
	for ( QMap< QString, QgsMapLayer* >::iterator it_layer = layers.begin( );
		it_layer != layers.end( ); ++it_layer )
	{
		if ( it_layer.value( )->name( ) == el.layer_id( ) )
		{
			if ( QgsVectorLayer *cur_layrer = qobject_cast< QgsVectorLayer* >( it_layer.value( ) ) )
			{
				QSet< int > sel_f;
				sel_f.insert( el.feature_id( ) );
				cur_layrer->setSelectedFeatures( sel_f );

				QgsRectangle next_rect;
				if ( zoom )
				{
					next_rect = m_manager->qgis_iface( )->mapCanvas( )->mapRenderer( )->layerExtentToOutputExtent( cur_layrer, cur_layrer->boundingBoxOfSelected( ) );
					if ( !next_rect.isEmpty( ) )
						next_rect.scale( 1.05 );
				}
				else
				{
					const QgsRectangle sel_rect = m_manager->qgis_iface( )->mapCanvas( )->mapRenderer( )->layerExtentToOutputExtent( cur_layrer, cur_layrer->boundingBoxOfSelected( ) );
					if ( !sel_rect.isEmpty( ) )
					{
						next_rect = m_manager->qgis_iface( )->mapCanvas( )->extent( );
						QgsPoint pt = sel_rect.center( );
						next_rect.scale( 1.0, &pt );
					}
				}
				if ( !next_rect.isEmpty( ) )
					m_manager->qgis_iface( )->mapCanvas( )->setExtent( next_rect );
				m_manager->qgis_iface( )->mapCanvas( )->refresh( );
			}
			return;
		}
	}
}

void pa_db::map_pa_elements_list::select( const QVector< pa_element > &els, bool zoom )
{
	QgsRectangle results_sel_rect;
	QMap< QString, QgsMapLayer* > layers = QgsMapLayerRegistry::instance( )->mapLayers( );
	for ( QMap< QString, QgsMapLayer* >::iterator it_layer = layers.begin( );
		it_layer != layers.end( ); ++it_layer )
	{
		if ( QgsVectorLayer *cur_layrer = qobject_cast< QgsVectorLayer* >( it_layer.value( ) ) )
		{
			QSet< int > sel_features;
			for ( int nn = 0; nn < els.count( ); ++nn )
			{
				if ( els[ nn ].layer_id( ) == it_layer.value( )->name( ) )
					sel_features.insert( els[ nn ].feature_id( ) );
			}
			if ( sel_features.count( ) )
			{
				cur_layrer->setSelectedFeatures( sel_features );
				QgsRectangle sel_rect = m_manager->qgis_iface( )->mapCanvas( )->mapRenderer( )->layerExtentToOutputExtent( cur_layrer, cur_layrer->boundingBoxOfSelected( ) );
				if ( !sel_rect.isEmpty( ) )
				{
					if ( results_sel_rect.isEmpty( ) )
						results_sel_rect = sel_rect;
					else
						results_sel_rect.combineExtentWith( &sel_rect );
				}

			}
		}
	}
	if ( !results_sel_rect.isEmpty( ) )
	{
		if ( zoom )
		{
			results_sel_rect.scale( 1.05 );
			m_manager->qgis_iface( )->mapCanvas( )->setExtent( results_sel_rect );
		}
		else
		{
			QgsRectangle cur_rect = m_manager->qgis_iface( )->mapCanvas( )->extent( );
			QgsPoint pt = results_sel_rect.center( );
			cur_rect.scale( 1.05, &pt );
			m_manager->qgis_iface( )->mapCanvas( )->setExtent( cur_rect );
		}
	}
	m_manager->qgis_iface( )->mapCanvas( )->refresh( );
}

bool pa_db::map_pa_elements_list::is_element_valid( const pa_element &el ) const
{
//	QMessageBox::warning( 0, "0", "0" );
	const bool result = std::find_if( constBegin( ), constEnd( ),
		find_element( el.layer_id( ), el.feature_id( ) ) ) != constEnd( );
//	QMessageBox::warning( 0, "1", "1" );
	return result;
}

QSet< QPair< QString, QString > > fill_element_data( const pa_db::pa_element &el, QgsVectorLayer *layer, const QMap< QString, QVariant > &data )
{
	QSet< QPair< QString, QString > > result;
	for ( QMap< QString, QVariant >::const_iterator it = data.constBegin( );
		it != data.constEnd( ); ++it )
	{
		QgsVectorDataProvider *data_provider = layer->dataProvider( );
		int field_id = data_provider->fieldNameIndex( it.key( ) );
		if ( field_id != -1 )
		{
			// key = field index, value = field value
			QMap< int, QVariant > attr_map;
			attr_map[ field_id ] = it.value( );

			// key = feature id, value = changed attributes
			QMap< int, QgsAttributeMap > change_attr;
			change_attr[ el.feature_id( ) ] = attr_map;
			data_provider->changeAttributeValues( change_attr );
		}
		else
		{
			result.insert( qMakePair( layer->name( ), it.key( ) ) );
		}
	}
	return result;
}

void pa_db::map_pa_elements_list::fill_elements_data( const QVector< pa_element > &els, const QMap< QString, QVariant > &data )
{
	QSet< QPair< QString, QString > > not_found;
	QgsRectangle results_sel_rect;
	QMap< QString, QgsMapLayer* > layers = QgsMapLayerRegistry::instance( )->mapLayers( );
	for ( QMap< QString, QgsMapLayer* >::iterator it_layer = layers.begin( );
		it_layer != layers.end( ); ++it_layer )
	{
		if ( QgsVectorLayer *cur_layer = qobject_cast< QgsVectorLayer* >( it_layer.value( ) ) )
		{
			for ( int nn = 0; nn < els.count( ); ++nn )
			{
				if ( els[ nn ].layer_id( ) == cur_layer->name( ) )
				{
					not_found += fill_element_data( els[ nn ], cur_layer, data );
				}
			}
		}
	}
	if ( not_found.count( ) )
	{
		QString str_not_found;
		for( QSet< QPair< QString, QString > >::const_iterator it = not_found.constBegin( );
			it != not_found.constEnd( ); ++it )
			str_not_found.append( QString( "\n" ) + it->first + it->second );
		QMessageBox::warning( m_manager->qgis_iface( )->mainWindow( ), QObject::tr( "Fields not found" ),
			QObject::tr( "Fields not found, list:" ) + str_not_found );
	}
}
