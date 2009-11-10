#pragma once
#include <QObject>

namespace pa_db
{

	class signal_blocker
	{
	public:
		signal_blocker( QObject* object );
		~signal_blocker( );
	private:
		QObject*	mObject;
	};

}