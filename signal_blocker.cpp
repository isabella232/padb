#include "signal_blocker.h"

pa_db::signal_blocker::signal_blocker( QObject* object )
	: mObject( object )
{
	mObject->blockSignals( true );
}

pa_db::signal_blocker::~signal_blocker( )
{
	mObject->blockSignals( false );
}
