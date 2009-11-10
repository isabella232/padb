#include "pa_object.h"
#include <map>

pa_db::pa_object::pa_object( )
	: m_id( -1 )
{
}

int pa_db::pa_object::id( ) const
{
	return m_id;
}

bool pa_db::pa_object::is_empty( ) const
{
	return id( ) == -1;
}

void pa_db::pa_object::set_id( int id )
{
	m_id = id;
}

QString pa_db::pa_object::name( ) const
{
	return m_name;
}

void pa_db::pa_object::set_name( const QString &name )
{
	m_name = name;
}

QVector< pa_db::pa_element > pa_db::pa_object::elements( ) const
{
	return m_elements;
}

void pa_db::pa_object::set_elements( const QVector< pa_db::pa_element >& list )
{
	m_elements = list;
}

QVector< pa_db::pa_species > pa_db::pa_object::species() const
{
	return mSpecies;
}

void pa_db::pa_object::setSpecies( const QVector< pa_species >& list )
{
	mSpecies = list;
}
