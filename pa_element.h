#pragma once
#include <QVector>
#include <QString>
class QDataStream;

namespace pa_db
{
	class pa_element
	{
	public:
		pa_element( );

		QString layer_id( ) const;
		void set_layer_id( const QString &layer_id );

		int feature_id( ) const;
		void set_feature_id( int id );

		QString feature_type( ) const;
		void set_feature_type( const QString &type );

		QByteArray to_byte_array( ) const;
		static pa_element from_byte_array( const QByteArray &ar );

	private:
		QString m_layer_id;
		QString m_feature_type;
		int		m_feature_id;
	};

	class find_element
	{
		int		m_feature_id;
		QString	m_layer_id;
	public:
		find_element( const QString &layer_id, int feature_id )
			: m_layer_id( layer_id )
			, m_feature_id( feature_id )
		{
		}
		bool operator ( )( const pa_db::pa_element &obj ) const
		{
			return obj.layer_id( ) == m_layer_id && obj.feature_id( ) == m_feature_id;
		}
	};

}

