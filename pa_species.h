#ifndef PASPECIES_H
#define PASPECIES_H

#include <QString>

class QDataStream;

namespace pa_db
{
  class pa_species
  {
    public:
      pa_species();
      
      int id() const;
      void setId( int id );
      
      int parentId() const;
      void setParentId( int id );
      
      QString caption() const;
      void setCaption( const QString &caption );
      
      QString nameRu() const;
      void setNameRu( const QString &name );
      
      QString nameEn() const;
      void setNameEn( const QString &name );
      
      QString comment() const;
      void setComment( const QString &comment );
      
      QString status() const;
      void setStatus( const QString &status );
      
      QByteArray toByteArray() const;
      static pa_species fromByteArray( const QByteArray &ar );
    
    private:
      int mId;
      int mParentId;
      QString mCaption;
      QString mNameRu;
      QString mNameEn;
      QString mComment;
      QString mStatus;
  };
}

#endif // PASPECIES_H
