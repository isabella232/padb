#include "pa_element.h"
#include <QDataStream>
#include "resources.h"

QDataStream& operator<<( QDataStream &out, const pa_db::pa_element &el );
QDataStream& operator>>( QDataStream &in, pa_db::pa_element &el );

pa_db::pa_element::pa_element( )
	: m_feature_id( -1 )
{
}
QString pa_db::pa_element::layer_id( ) const
{
	return m_layer_id.left( pa_db::def::text_max_len );
}
void pa_db::pa_element::set_layer_id( const QString &layer_id )
{
	m_layer_id = layer_id.left( pa_db::def::text_max_len );
}
int pa_db::pa_element::feature_id( ) const
{
	return m_feature_id;
}
void pa_db::pa_element::set_feature_id( int id )
{
	m_feature_id = id;
}
QString pa_db::pa_element::feature_type( ) const
{
	return m_feature_type;
}
void pa_db::pa_element::set_feature_type( const QString &type )
{
	m_feature_type = type;
}

QByteArray pa_db::pa_element::to_byte_array( ) const
{
	QByteArray mime_array;
	QDataStream sender( &mime_array, QIODevice::WriteOnly );
	sender << *this;
	return mime_array;
}

pa_db::pa_element pa_db::pa_element::from_byte_array( const QByteArray &ar )
{
	pa_element result;
	QDataStream stream( ar );
	stream >> result;
	return result;
}


QDataStream& operator<<( QDataStream &out, const pa_db::pa_element &el )
{
	out << el.layer_id( ) << el.feature_id( ) << el.feature_type( );
	return out;
}

QDataStream& operator>>( QDataStream &in, pa_db::pa_element &el )
{
	QString layer_id, feature_type;
	int feature_id;
	in >> layer_id >> feature_id >> feature_type;
	el.set_layer_id( layer_id );
	el.set_feature_id( feature_id );
	el.set_feature_type( feature_type );
	return in;
}


