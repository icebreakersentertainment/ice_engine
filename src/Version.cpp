#include "Version.hpp"

#include "data_store/SqliteDataStore.hpp"

namespace game
{

Version::Version(int id, int version) : id_(id), version_(version)
{
}

Version::~Version()
{
}

// @serialize start
void Version::serialize(SQLite::Database& db, pyliteserializer::SqliteDataStore& ds)
{
	std::stringstream ss;
	if (id_ > 0)
	{
		ss << "UPDATE version SET id=" << id_ << ", version=" << version_;
	}
	else
	{
		ss << "INSERT INTO version (id, version) VALUES (";
		ss << id_ << ", " << version_;
		ss << ")";
	}
	if (id_ > 0)
	{
		ss << " WHERE id = " << id_;
	}
	
	try
	{
		db.exec( ss.str().c_str() );
	}
	catch (std::exception& e)
	{
		std::cout << "SQLite Exception: " << e.what() << std::endl;
		std::cout << "Query: " << ss.str() << std::endl;
	}
	
	save(ds);
}
// @serialize end

// @deserialize start
void Version::deserialize(SQLite::Database& db, pyliteserializer::SqliteDataStore& ds)
{
	SQLite::Statement query(db, "SELECT id, version FROM version WHERE id = ?");

	query.bind(1, id_);

	while (query.executeStep())
	{
		id_ = query.getColumn(0);
		version_ = query.getColumn(1);
	}
	
	load(ds);
}
// @deserialize end

// @deserialize_where start
void Version::deserialize(SQLite::Database& db, pyliteserializer::SqliteDataStore& ds, const std::string& where)
{
	std::string sql = std::string();
	
	if (where.length() > 0)
		sql = std::string( "SELECT id, version FROM version WHERE " + where );
	else
		sql = std::string( "SELECT id, version FROM version WHERE id = ?" );

	SQLite::Statement query(db,  sql.c_str());

	query.bind(1, id_);

	while (query.executeStep())
	{
		id_ = query.getColumn(0);
		version_ = query.getColumn(1);
	}
	
	load(ds);
}
// @deserialize_where end

// @deserialize_from_query start
void Version::deserialize(SQLite::Statement& query, pyliteserializer::SqliteDataStore& ds)
{
	id_ = query.getColumn(0);
	version_ = query.getColumn(1);
	
	load(ds);
}
// @deserialize_from_query end

}
