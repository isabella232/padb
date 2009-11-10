#pragma once
#include <QDialog>
class QLineEdit;
class QGroupBox;
class QPushButton;
class QTableWidget;
class QComboBox;
class QgisInterface;

namespace pa_db
{
	class settings_form : public QDialog
	{
		Q_OBJECT
	public:
		settings_form( QWidget *parent );

	private Q_SLOTS:
		void on_cancel( );
		void on_ok( );
		void on_open_path( );
		void on_cell_double_clicked( int row, int column );

	private:
		void create_db_settings( QGroupBox *db_box );
		void on_allpy( );
		void fill_mapping_table( );
		void apply_mapping_table( ) const;
		void fill_layrs_list( );

		QLineEdit		*m_db_path;
		QLineEdit		*m_min_zoom;
		QTableWidget	*m_data_mapping;
		QComboBox		*m_exp_layer;
	};
}