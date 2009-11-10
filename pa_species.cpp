#include <QDataStream>

#include "pa_species.h"
#include "resources.h"

QDataStream& operator<<( QDataStream &out, const pa_db::pa_species &el );
QDataStream& operator>>( QDataStream &in, pa_db::pa_species &el );

pa_db::pa_species::pa_species()
{
//  mParentId = -1;
}

int pa_db::pa_species::id() const
{
  return mId;
}

void pa_db::pa_species::setId( int id )
{
  mId = id;
}

int pa_db::pa_species::parentId() const
{
  return mParentId;
}

void pa_db::pa_species::setParentId( int id )
{
  mParentId = id;
}

QString pa_db::pa_species::caption() const
{
  return mCaption;
}

void pa_db::pa_species::setCaption( const QString &caption )
{
  mCaption = caption;
}

QString pa_db::pa_species::nameRu() const
{
  return mNameRu;
}

void pa_db::pa_species::setNameRu( const QString &name )
{
  mNameRu = name;
}

QString pa_db::pa_species::nameEn() const
{
  return mNameEn;
}

void pa_db::pa_species::setNameEn( const QString &name )
{
  mNameEn = name;
}

QString pa_db::pa_species::comment() const
{
  return mComment;
}

void pa_db::pa_species::setComment( const QString &comment )
{
  mComment = comment;
}

QString pa_db::pa_species::status() const
{
  return mStatus;
}

void pa_db::pa_species::setStatus( const QString &status )
{
  mStatus = status;
}

QByteArray pa_db::pa_species::toByteArray() const
{
  QByteArray array;
  QDataStream sender( &array, QIODevice::WriteOnly );
  sender << *this;
  return array;
}

pa_db::pa_species pa_db::pa_species::fromByteArray( const QByteArray &ar )
{
  pa_species result;
  QDataStream stream( ar );
  stream >> result;
  return result;
}

QDataStream& operator<<( QDataStream &out, const pa_db::pa_species &el )
{
  out << el.id() << el.parentId() << el.caption() << el.nameRu() << el.nameEn() << el.comment() << el.status();
  return out;
}

QDataStream& operator>>( QDataStream &in, pa_db::pa_species &el )
{
  QString caption, nameRu, nameEn, comment, status;
  int id, parentId;
  in >> id >> parentId >> caption >> nameRu >> nameEn >> comment >> status;
  el.setId( id );
  el.setParentId( parentId );
  el.setCaption( caption );
  el.setNameRu( nameRu );
  el.setNameEn( nameEn );
  el.setComment( comment );
  el.setStatus( status );
  return in;
}
