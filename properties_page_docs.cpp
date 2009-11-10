#include "properties_page_docs.h"
#include "resources.h"
#include "signal_blocker.h"
#include <QHBoxLayout>
#include <QListView>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
#include <QDataWidgetMapper>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlRecord>
#include <QUrl>
#include <QDate>
#include <QLineEdit>
#include <QMessageBox>
#include <QDesktopServices>

pa_db::properties_page_docs::properties_page_docs( int object_id, QWidget *parent )
	: properties_page_base( object_id, parent )
	, m_docs_table( 0 )
{
	setup_ui( );
	if ( m_database.isOpen( ) )
	{
		m_docs_table = new QSqlTableModel( this, m_database );
		m_docs_table->setTable( "docs" );
		m_docs_table->setEditStrategy( QSqlTableModel::OnManualSubmit );
		m_docs_table->setFilter( QString( "object_id=%1" ).arg( object_id ) );
		m_docs_table->setSort( list_view_column( ), Qt::AscendingOrder );
		m_docs_table->select( );
		m_docs_list->setModel( m_docs_table );
		m_docs_list->setModelColumn( list_view_column( ) );
		connect( m_docs_table, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ),
			SIGNAL( changed( ) ) );
		connect( m_docs_list->selectionModel( ),
			SIGNAL( currentChanged ( const QModelIndex&, const QModelIndex& ) ),
			SLOT( on_change_select_doc( ) ) );

		m_table_mapper = new QDataWidgetMapper( this );
		m_table_mapper->setModel( m_docs_table );

		Q_ASSERT( m_docs_table->fieldIndex( "date" ) != -1 );
		m_table_mapper->addMapping( m_date_label, m_docs_table->fieldIndex( "date" ) );

		Q_ASSERT( m_docs_table->fieldIndex( "url" ) != -1 );
		m_table_mapper->addMapping( m_url_label, m_docs_table->fieldIndex( "url" ) );

		Q_ASSERT( m_docs_table->fieldIndex( "description" ) != -1 );
		m_table_mapper->addMapping( m_description, m_docs_table->fieldIndex( "description" ) );

		if ( m_docs_table->rowCount( ) != 0 )
			select_row( 0 );

	}
	else
		qDebug( ) << "db not open";
}

QString pa_db::properties_page_docs::page_name( ) const
{
	return text::page_title_docs( );
}

void pa_db::properties_page_docs::apply( )
{
	m_docs_table->submitAll( );
}

void pa_db::properties_page_docs::on_add_doc( )
{
	const QString file_url = QFileDialog::getOpenFileName( this );
	if ( !file_url.isEmpty( ) )
	{
		QSqlRecord ins_rec = m_docs_table->record( );
		ins_rec.setValue( "url", file_url );
		ins_rec.setValue( "title", QFileInfo( file_url ).fileName( ) );
		ins_rec.setValue( "object_id", m_object_id );
		m_docs_table->insertRecord( -1, ins_rec );
		select_row( m_docs_table->rowCount( ) - 1 );
		Q_EMIT changed( );
	}
}

void pa_db::properties_page_docs::on_remove_doc( )
{
	const QModelIndex current = m_docs_list->currentIndex( );
	if ( current.isValid( ) )
	{
		const int row = current.row( );
		bool res = m_docs_table->removeRows( current.row( ), 1 );
		m_docs_list->update( current );
		Q_EMIT changed( );
	}
}

void pa_db::properties_page_docs::setup_ui( )
{
	QHBoxLayout *main_layout = new QHBoxLayout( this );

	QVBoxLayout *docs_layout = new QVBoxLayout;
	main_layout->addLayout( docs_layout, 0 );

	m_docs_list = new QListView( this );
	docs_layout->addWidget( m_docs_list );
	m_docs_list->setMaximumWidth( 200 );
	m_docs_list->setMinimumWidth( 200 );
	m_docs_list->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

	QHBoxLayout *buttons_layout = new QHBoxLayout;
	docs_layout->addLayout( buttons_layout );

	QPushButton *bt_add_doc = new QPushButton( text::add( ), this );
	buttons_layout->addWidget( bt_add_doc );
	connect( bt_add_doc, SIGNAL( clicked( bool ) ), SLOT( on_add_doc( ) ) );

	QPushButton *bt_remove_doc = new QPushButton( text::remove( ), this );
	buttons_layout->addWidget( bt_remove_doc );
	connect( bt_remove_doc, SIGNAL( clicked( bool ) ), SLOT( on_remove_doc( ) ) );

	buttons_layout->addStretch( );

	QGridLayout *desc_layout = new QGridLayout;
	main_layout->addLayout( desc_layout, 1 );

	int cur_row = 0;
	desc_layout->addWidget( new QLabel( text::date( ), this ), cur_row, 0 );
	desc_layout->addWidget( m_date_label = new QLineEdit( "?", this ), cur_row, 1 );
	m_date_label->setReadOnly( true );
	m_date_label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
	cur_row++;

	desc_layout->addWidget( new QLabel( text::url( ), this ), cur_row, 0 );
	desc_layout->addWidget( m_url_label = new QLineEdit( "?", this ), cur_row, 1 );
	m_url_label->setReadOnly( true );
	m_url_label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
	cur_row++;

	desc_layout->addWidget( new QLabel( text::status( ), this ), cur_row, 0 );
	desc_layout->addWidget( m_status_label = new QLabel( "?", this ), cur_row, 1 );
	m_status_label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
	cur_row++;

	desc_layout->addWidget( new QLabel( text::file_description( ), this ), cur_row++, 0 );

	desc_layout->addWidget( m_description = new QTextEdit( this ), cur_row++, 0, 1, 2 );
	m_description->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

	QHBoxLayout *open_button_layout = new QHBoxLayout;
	desc_layout->addLayout( open_button_layout, cur_row, 0, 1, 2 );

	QPushButton *bt_open_doc = new QPushButton( text::open( ), this );
	open_button_layout->addWidget( bt_open_doc );
	connect( bt_open_doc, SIGNAL( clicked( bool ) ), SLOT( on_open_doc( ) ) );
	open_button_layout->addStretch( 1 );
}

void pa_db::properties_page_docs::on_open_doc( )
{
	const QString url = m_url_label->text( );
	if ( QFile::exists( m_url_label->text( ) ) )
	{
		if ( !QDesktopServices::openUrl( "file:///" + url ) )
			QMessageBox::warning( this, text::pa_db_plugin_name( ), text::cannot_open_file( ) );
	}
	else
		QMessageBox::information( this, text::pa_db_plugin_name( ), text::file_not_exist( ) );
}

void pa_db::properties_page_docs::on_change_select_doc( )
{
	const QModelIndex current = m_docs_list->currentIndex( );
	if ( current.isValid( ) )
	{
		m_table_mapper->setCurrentIndex( current.row( ) );
		m_status_label->setText(
			QFile::exists( m_docs_table->record( current.row( ) ).value( "url" ).toString( ) )
			? text::found( )
			: "<font color=ff0000>" + text::not_found( ) + "</font>" );

	}
}

int pa_db::properties_page_docs::list_view_column( )
{
	Q_ASSERT( m_docs_table->fieldIndex( "title" ) != -1 );
	return m_docs_table->fieldIndex( "title" );
}

void pa_db::properties_page_docs::select_row( int row )
{
	const QModelIndex sel_el = m_docs_table->index( row, list_view_column( ) );
	if ( sel_el.isValid( ) )
	{
		m_docs_list->selectionModel( )->reset( );
		m_docs_list->selectionModel( )->setCurrentIndex( sel_el, QItemSelectionModel::SelectCurrent );
	}
}

