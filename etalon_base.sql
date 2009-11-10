-- справочники
CREATE TABLE category
(
  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  name VARCHAR(255) NOT NULL
);

CREATE TABLE profile
(
  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  name VARCHAR(255) NOT NULL
);

CREATE TABLE status
(
  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  name VARCHAR(255) NOT NULL
);

CREATE TABLE species
(
  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  nameRU VARCHAR(255),
  nameEN VARCHAR(255),
  comment VARCHAR(255),
  protectstatus VARCHAR(255)
);

-- основные таблицы
CREATE TABLE objects
(
  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  name VARCHAR(255) NOT NULL,
  entity_number VARCHAR(255) DEFAULT NULL,
  okato VARCHAR(255) DEFAULT NULL,
  cluster INTEGER DEFAULT '0',
  locate TEXT DEFAULT NULL,
  category_id INTEGER DEFAULT '1' NOT NULL CONSTRAINT fk_obj_category REFERENCES category( id ),
  profile_id INTEGER DEFAULT '1' NOT NULL CONSTRAINT fk_obj_profile REFERENCES profile( id ),
  status_id INTEGER DEFAULT '1' NOT NULL CONSTRAINT fk_obj_status REFERENCES status( id ),
  type_id INTEGER DEFAULT '1' NOT NULL CONSTRAINT fk_obj_type REFERENCES type( id ),
  [year] INTEGER DEFAULT NULL,
  area INTEGER DEFAULT NULL,
  norma TEXT DEFAULT NULL,
  osnova TEXT DEFAULT NULL,
  inter TEXT DEFAULT NULL,
  list TEXT DEFAULT NULL,
  subst TEXT DEFAULT NULL,
  address TEXT DEFAULT NULL,
  substpa TEXT DEFAULT NULL,
  geogr TEXT DEFAULT NULL,
  relief TEXT DEFAULT NULL,
  climate TEXT DEFAULT NULL,
  veg TEXT DEFAULT NULL,
  flora TEXT DEFAULT NULL,
  fauna TEXT DEFAULT NULL,
  explicat TEXT DEFAULT NULL,
  zoning TEXT DEFAULT NULL,
  buffer TEXT DEFAULT NULL,
  border TEXT DEFAULT NULL,
  objects TEXT DEFAULT NULL,
  measures TEXT DEFAULT NULL,
  suggest TEXT DEFAULT NULL,
  number INTEGER DEFAULT '0' NOT NULL,
  codes TEXT DEFAULT NULL,
  fulladdr TEXT DEFAULT NULL,
  phone VARCHAR(255) DEFAULT NULL,
  email VARCHAR(255) DEFAULT NULL,
  fio VARCHAR(255) DEFAULT NULL,
  staff TEXT DEFAULT NULL,
  protect VARCHAR(255) DEFAULT NULL,
  protectbuffer TEXT DEFAULT NULL,
  science TEXT DEFAULT NULL,
  educat TEXT DEFAULT NULL,
  cost TEXT DEFAULT NULL,
  [add] TEXT DEFAULT NULL,
  creator VARCHAR(255) DEFAULT NULL
);

CREATE TABLE docs
(
  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  object_id INTEGER NOT NULL CONSTRAINT fk_docs_obj REFERENCES objects( id ),
  url VARCHAR(255) NOT NULL,
  description VARCHAR(255) DEFAULT NULL,
  [date] DATE DEFAULT CURRENT_DATE,
  title VARCHAR(255) NOT NULL
);

CREATE TABLE links
(
  object_id INTEGER NOT NULL CONSTRAINT fk_links_obj REFERENCES objects( id ),
  layer_id VARCHAR(255) NOT NULL,
  feature_id INTEGER NOT NULL,
  feature_type VARCHAR(255) NOT NULL
);

CREATE TABLE link_species
(
  objectId INTEGER NOT NULL CONSTRAINT fk_link_obj REFERENCES objects( id ),
  speciesId INTEGER NOT NULL CONSTRAINT fk_link_species REFERENCES species( id )
);


CREATE VIEW IF NOT EXISTS vlinks AS
  SELECT
    l.objectId AS objectId,
    s.id AS speciesId,
    s.nameRu AS species_nameRu,
    s.nameEn AS species_nameEn,
    s.comment AS species_comment,
    s.protectstatus AS species_status
  FROM
    link_species l LEFT JOIN species s ON l.speciesId = s.id;

