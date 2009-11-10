#include <QVBoxLayout>
#include <QLabel>
#include <QSplitter>

#include "dock_widget.h"
#include "objects_tree_widget.h"
#include "free_elements_widget.h"
#include "species_widget.h"
#include "resources.h"

pa_db::dock_widget::dock_widget( const QString& title, QWidget *parent )
	: QDockWidget( title, parent )
	, m_first_text( 0 )
	, m_second_text( 0 )
{
	QSplitter *sp = new QSplitter( Qt::Vertical, this );
	setWidget( sp );

	QWidget *up_part = new QWidget( sp );
	sp->addWidget( up_part );
	create_up_part( up_part );

	QWidget *mid_part = new QWidget( sp );
	sp->addWidget( mid_part );
	create_mid_part( mid_part );

	QWidget *bottom_part = new QWidget( sp );
	sp->addWidget( bottom_part );
	create_bottom_part( bottom_part );
}

void pa_db::dock_widget::create_up_part( QWidget *parent )
{
	QVBoxLayout *main_layout = new QVBoxLayout( parent );
	main_layout->setContentsMargins( 0, 0, 0, 0 );
	main_layout->addWidget( m_first_text = new QLabel( text::objects_list( ), parent ) );
	m_objects_tree = new objects_tree_widget( parent );
	main_layout->addWidget( m_objects_tree );
}

void pa_db::dock_widget::create_mid_part( QWidget *parent )
{
	QVBoxLayout *main_layout = new QVBoxLayout( parent );
	main_layout->setContentsMargins( 0, 0, 0, 0 );
	main_layout->addWidget( lblSpeciesList = new QLabel( tr( "Species" ), parent ) );
	main_layout->addWidget( mSpeciesList = new species_widget( parent ) );
}

void pa_db::dock_widget::create_bottom_part( QWidget *parent )
{
	QVBoxLayout *main_layout = new QVBoxLayout( parent );
	main_layout->setContentsMargins( 0, 0, 0, 0 );
	main_layout->addWidget( m_second_text = new QLabel( text::free_elements_list( ), parent ) );
	main_layout->addWidget( m_free_elements_list = new free_elements_widget( parent ) );
}

pa_db::objects_tree_widget *pa_db::dock_widget::objects_tree( )
{
	return m_objects_tree;
}

pa_db::free_elements_widget *pa_db::dock_widget::free_elements_list( )
{
	return m_free_elements_list;
}

pa_db::species_widget *pa_db::dock_widget::speciesList( )
{
	return mSpeciesList;
}

void pa_db::dock_widget::show_error_message( )
{
	objects_tree( )->hide( );
	free_elements_list( )->hide( );
	speciesList( )->hide( );
	m_first_text->setText( text::error_open_db( ) );
	m_second_text->hide( );
	lblSpeciesList->hide( );
}

void pa_db::dock_widget::hideSpecies()
{
	QWidget *spw = qobject_cast<QWidget *>( lblSpeciesList->parent() );
	spw->hide();
	
	//speciesList()->hide();
	//lblSpeciesList->hide();
}

void pa_db::dock_widget::showSpecies()
{
	QWidget *spw = qobject_cast<QWidget *>( lblSpeciesList->parent() );
	spw->show();
	
	//speciesList()->show();
	//lblSpeciesList->show();
}
