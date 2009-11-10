#pragma once

namespace pa_db
{
	namespace text
	{
		inline QString map_all_objects( )			{ return QObject::tr( "Dump from the database to shapefile" ); }
		inline QString message_no_layer( )			{ return QObject::tr( "Objects layer does not exist in the list. Please check Modules\\PADB\\Settings\\Objects layer" ); }
		inline QString exp_layer( )					{ return QObject::tr( "Objects layer" ); }
		inline QString field1( )					{ return QObject::tr( "DB" ); }
		inline QString field2( )					{ return QObject::tr( "Shapefile" ); }
		inline QString db_mapping_comment( )		{ return QObject::tr( "Select fields in the database and matching fields in the shapefile" ); }
		inline QString locate_db_file( )			{ return QObject::tr( "Locate existing or enter new filename for DB including path" ); }
		inline QString mapping( )					{ return QObject::tr( "Mapping" ); }
		inline QString db_mapping( )				{ return QObject::tr( "Field mapping:" ); }
		inline QString min_zoom( )					{ return QObject::tr( "Max zoom:" ); }
		inline QString error_create_empty_db( )		{ return QObject::tr( "Error creating empty DB" ); }
		inline QString error_open_db( )				{ return QObject::tr( "Error opening DB.\nIf this is the first time you open PADB,\nplease go to Modules\\PADB\\Settings\nand locate existing or create new DB.\nYou'll need to restart QGIS or PADB\nfor changes to take effect." ); }
		inline QString group_box_db( )				{ return QObject::tr( "DB Location" ); }
		inline QString group_box_zoom( )			{ return QObject::tr( "Printout zoom" ); }
		inline QString about( )						{ return QObject::tr( "About" ); }
		inline QString add( )						{ return QObject::tr( "Add" ); }
		inline QString apply( )						{ return QObject::tr( "Apply" ); }
		inline QString cancel( )					{ return QObject::tr( "Cancel" ); }
		inline QString cluster( )					{ return QObject::tr( "Cluster:" ); }
		inline QString create_object( )				{ return QObject::tr( "Create object" ); }
		inline QString select_object( )				{ return QObject::tr( "Select object" ); }
		inline QString database_connection_name( )	{ return "pa_db_plugin_database_connection"; }
		inline QString date( )						{ return QObject::tr( "Date:" ); }
		inline QString developers( )				{ return QObject::tr( "Developers:" ); }
		inline QString db_file_name( )				{ return "pa_db_data.db3"; }
		inline QString db_file( )					{ return QObject::tr( "DB file:" ); }
		inline QString delete_object( )				{ return QObject::tr( "Delete" ); }
		inline QString description( )				{ return QObject::tr( "Interface to work with protected areas." ); }
		inline QString entity_number( )				{ return QObject::tr( "Entity number:" ); }
		inline QString free_element_mime_data( )	{ return "application/x-qt-windows-mime;value=\"free_element_mime_data\""; }
		inline QString free_elements_list( )		{ return QObject::tr( "Unlinked elements" ); }
		inline QString input_new_object_name( )		{ return QObject::tr( "New object name" ); }
		inline QString kod_okato( )					{ return QObject::tr( "OCATO code:" ); }
		inline QString layer_filter_name( )			{ return "oopt-poly"; }
		inline QString link_element_mime_data( )	{ return "application/x-qt-windows-mime;value=\"link_element_mime_data\""; }
		inline QString load_object_list( )			{ return QObject::tr( "Update objects list" ); }
		inline QString locate( )					{ return QObject::tr( "Locate:" ); }
		inline QString link( )						{ return QObject::tr( "More info:" ); }
		inline QString objects_list( )				{ return QObject::tr( "Objects" ); }
		inline QString ok( )						{ return QObject::tr( "OK" ); }
		inline QString open_file( )					{ return QObject::tr( "Open file" ); }
		inline QString open_file_filter( )			{ return QObject::tr( "DB files (*.db3 *.db);;All files (*.*)" ); }
		inline QString pa_db_plugin_name( )			{ return QObject::tr( "PADB" ); }
		inline QString pa_db_plugin_name_in_dock_widget( )		{ return QObject::tr( "Manage" ); }
		inline QString pa_db_plugin_name_in_tool_bar( )			{ return "PADB"; }
		inline QString pa_db_plugin_name_in_plugin_manager( )	{ return "PADB"; }
		inline QString page_title_activity( )		{ return QObject::tr( "Activity" ); }
		inline QString page_title_docs( )			{ return QObject::tr( "Documents" ); }
		inline QString page_title_general( )		{ return QObject::tr( "General" ); }
		inline QString page_title_importance( )		{ return QObject::tr( "Importance" ); }
		inline QString page_title_nature( )			{ return QObject::tr( "Nature" ); }
		inline QString page_title_organization( )	{ return QObject::tr( "Organization" ); }
		inline QString page_title_regime( )			{ return QObject::tr( "Regime" ); }
		inline QString page_title_subjection( )		{ return QObject::tr( "Subjection" ); }
		inline QString properties( )				{ return QObject::tr( "Properties" ); }
		inline QString properties_name( )			{ return QObject::tr( "Name:" ); }
		inline QString question_delete_object( )	{ return QObject::tr( "Delete object %1?" ); }
		inline QString question_create_empty_db( )	{ return QObject::tr( "DB file does not exist, create empty db?" ); }
		inline QString remove( )					{ return QObject::tr( "Remove" ); }
		inline QString select( )					{ return QObject::tr( "Select" ); }
		inline QString select_and_zoom( )			{ return QObject::tr( "Select and zoom to" ); }
		inline QString settings( )					{ return QObject::tr( "Settings" ); }
		inline QString status( )					{ return QObject::tr( "Status:" ); }
		inline QString url( )						{ return QObject::tr( "URL:" ); }
		inline QString version( )					{ return QObject::tr( "Version: 0.1" ); }
		inline QString file_description( )			{ return QObject::tr( "Description:" ); }
		inline QString open( )						{ return QObject::tr( "Open" ); }
		inline QString cannot_open_file( )			{ return QObject::tr( "Cannot open file" ); }
		inline QString file_not_exist( )			{ return QObject::tr( "File not exist" ); }
		inline QString found( )						{ return QObject::tr( "FOUND" ); }
		inline QString not_found( )					{ return QObject::tr( "NOT_FOUND" ); }
		inline QString category( )					{ return QObject::tr( "Category:" ); }
		inline QString profile( )					{ return QObject::tr( "Profile:" ); }
		inline QString status_label( )				{ return QObject::tr( "Status:" ); }
		inline QString year_create( )				{ return QObject::tr( "Year created:" ); }
		inline QString year_short( )				{ return QObject::tr( "y." ); }
		inline QString area( )						{ return QObject::tr( "Area:" ); }
		inline QString area_short( )				{ return QObject::tr( "ha." ); }
		inline QString norma( )						{ return QObject::tr( "Norma base:" ); }
		inline QString osnova( )					{ return QObject::tr( "Osnova sozdania:" ); }
		inline QString diploms( )					{ return QObject::tr( "Diploms:" ); }
		inline QString sec_obj_list( )				{ return QObject::tr( "Sec. obj. list:" ); }
		inline QString subst( )						{ return QObject::tr( "Sub st:" ); }
		inline QString substpa( )					{ return QObject::tr( "Sub st pa:" ); }
		inline QString address( )					{ return QObject::tr( "Address:" ); }
		inline QString geogr( )						{ return QObject::tr( "Geogr:" ); }
		inline QString relief( )					{ return QObject::tr( "Relief:" ); }
		inline QString climate( )					{ return QObject::tr( "Climate:" ); }
		inline QString veg( )						{ return QObject::tr( "Veg:" ); }
		inline QString flora( )						{ return QObject::tr( "Flora:" ); }
		inline QString fauna( )						{ return QObject::tr( "Fauna:" ); }
		inline QString explicat( )					{ return QObject::tr( "Explicat:" ); }
		inline QString zoning( )					{ return QObject::tr( "Zoning:" ); }
		inline QString buffer( )					{ return QObject::tr( "Buffer:" ); }
		inline QString border( )					{ return QObject::tr( "Border:" ); }
		inline QString objects( )					{ return QObject::tr( "Objects:" ); }
		inline QString measures( )					{ return QObject::tr( "Measures:" ); }
		inline QString suggest( )					{ return QObject::tr( "Suggest:" ); }
		inline QString number( )					{ return QObject::tr( "Number:" ); }
		inline QString codes( )						{ return QObject::tr( "Codes:" ); }
		inline QString fulladdr( )					{ return QObject::tr( "Fulladdr:" ); }
		inline QString phone( )						{ return QObject::tr( "Phone:" ); }
		inline QString email( )						{ return QObject::tr( "Email:" ); }
		inline QString fio( )						{ return QObject::tr( "Fio:" ); }
		inline QString staff( )						{ return QObject::tr( "Staff:" ); }
		inline QString protect( )					{ return QObject::tr( "Protect:" ); }
		inline QString protectbuffer( )				{ return QObject::tr( "Protectbuffer:" ); }
		inline QString science( )					{ return QObject::tr( "Science:" ); }
		inline QString educat( )					{ return QObject::tr( "Educat:" ); }
		inline QString cost( )						{ return QObject::tr( "Cost:" ); }
		inline QString add_label( )					{ return QObject::tr( "Add:" ); }
		inline QString creator( )					{ return QObject::tr( "Creator:" ); }
		inline QString report( )					{ return QObject::tr( "Report" ); }
	}
	namespace icon
	{
		static const QString oopt_toolbar_add			( ":/pa_db/oopt_toolbar_add.png" );
		static const QString oopt_toolbar_list			( ":/pa_db/oopt_toolbar_list.png" );
		static const QString oopt_toolbar_select		( ":/pa_db/oopt_toolbar_select.png" );
		static const QString oopt_object_invalid		( ":/pa_db/oopt_object_invalid.png" );
		static const QString oopt_object_invalid_nolinks( ":/pa_db/oopt_object_invalid_nolinks.png" );
		static const QString oopt_object_valid			( ":/pa_db/oopt_object_valid.png" );
		static const QString oopt_element_invalid		( ":/pa_db/oopt_element_invalid.png" );
		static const QString oopt_element_unlinked		( ":/pa_db/oopt_element_unlinked.png" );
		static const QString oopt_element_valid			( ":/pa_db/oopt_element_valid.png" );

	}
	namespace def
	{
		static const int text_max_len = 254;
	}

	// запрос на создание базы
	/*
	CREATE TABLE XP_PROC
	-- This table created by OsenVistaSuite 2008
	-- Created date: 15.10.2008 1:14:42
	(
	view_name    TEXT,
	param_list   TEXT,
	xSQL         TEXT,
	def_param    TEXT,
	opt_param    TEXT,
	comment      TEXT,
	PRIMARY KEY (
	view_name
	)
	);

	CREATE TABLE [category] (
	[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
	[name] VARCHAR(255)  NOT NULL
	);

	CREATE TABLE [docs] (
	[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
	[object_id] INTEGER  NOT NULL,
	[url] VARCHAR(255)  NOT NULL,
	[description] TEXT  NULL,
	[date] DATE DEFAULT CURRENT_DATE NULL,
	[title] VARCHAR(255)  NOT NULL
	);

	CREATE TABLE [links] (
	[object_id] INTEGER  NOT NULL,
	[layer_id] VARCHAR(255)  NOT NULL,
	[feature_id] INTEGER  NOT NULL,
	[feature_type] VARCHAR(255)  NOT NULL
	);

	CREATE TABLE [objects] (
	[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
	[name] VARCHAR(255)  NOT NULL,
	[entity_number] VARCHAR(255)  NULL,
	[okato] VARCHAR(255)  NULL,
	[cluster] INTEGER DEFAULT '0' NULL,
	[locate] TEXT  NULL,
	[category_id] INTEGER DEFAULT '1' NOT NULL,
	[profile_id] INTEGER DEFAULT '1' NOT NULL,
	[status_id] INTEGER DEFAULT '1' NOT NULL,
	[year] INTEGER  NULL,
	[area] INTEGER  NULL,
	[norma] TEXT  NULL,
	[osnova] TEXT  NULL,
	[inter] TEXT  NULL,
	[list] TEXT  NULL,
	[subst] TEXT  NULL,
	[address] TEXT  NULL,
	[substpa] TEXT  NULL,
	[geogr] TEXT  NULL,
	[relief] TEXT  NULL,
	[climate] TEXT  NULL,
	[veg] TEXT  NULL,
	[flora] TEXT  NULL,
	[fauna] TEXT  NULL,
	[explicat] TEXT  NULL,
	[zoning] TEXT  NULL,
	[buffer] TEXT  NULL,
	[border] TEXT  NULL,
	[objects] TEXT  NULL,
	[measures] TEXT  NULL,
	[suggest] TEXT  NULL,
	[number] INTEGER DEFAULT '0' NULL,
	[codes] TEXT  NULL,
	[fulladdr] TEXT  NULL,
	[phone] VARCHAR(255)  NULL,
	[email] VARCHAR(255)  NULL,
	[fio] VARCHAR(255)  NULL,
	[staff] TEXT  NULL,
	[protect] VARCHAR(255)  NULL,
	[protectbuffer] TEXT  NULL,
	[science] TEXT  NULL,
	[educat] TEXT  NULL,
	[cost] TEXT  NULL,
	[add] TEXT  NULL,
	[creator] VARCHAR(255)  NULL
	);

	CREATE TABLE [profile] (
	[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
	[name] VARCHAR(255)  NOT NULL
	);

	CREATE TABLE [status] (
	[id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
	[name] VARCHAR(255)  NOT NULL
	);
	*/
}
