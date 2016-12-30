#include "data_store/SqliteDataStore.hpp"

#include "world/Region.hpp"

namespace game
{
namespace world
{
	
Region::Region(int id, std::string name, int worldId) : id_(id), name_(name), worldId_(worldId)
{
}

Region::~Region()
{
}

// @serialize start
void Region::serialize(SQLite::Database& db, pyliteserializer::SqliteDataStore& ds)
{
	std::stringstream ss;
	if (id_ > 0)
	{
		ss << "UPDATE region SET id=" << id_ << ", name=" << "'" << name_ << "'" << ", world_id=" << worldId_ << ", x=" << x_ << ", y=" << y_;
	}
	else
	{
		ss << "INSERT INTO region (id, name, world_id, x, y) VALUES (";
		ss << id_ << ", " << "'" << name_ << "'" << ", " << worldId_ << ", " << x_ << ", " << y_;
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
void Region::deserialize(SQLite::Database& db, pyliteserializer::SqliteDataStore& ds)
{
	SQLite::Statement query(db, "SELECT id, name, world_id, x, y FROM region WHERE id = ?");

	query.bind(1, id_);

	while (query.executeStep())
	{
		id_ = query.getColumn(0);
		name_ = std::string( query.getColumn(1).getText() );
		worldId_ = query.getColumn(2);
		x_ = query.getColumn(3);
		y_ = query.getColumn(4);
	}
	
	load(ds);
}
// @deserialize end

// @deserialize_where start
void Region::deserialize(SQLite::Database& db, pyliteserializer::SqliteDataStore& ds, const std::string& where)
{
	std::string sql = std::string();
	
	if (where.length() > 0)
		sql = std::string( "SELECT id, name, world_id, x, y FROM region WHERE " + where );
	else
		sql = std::string( "SELECT id, name, world_id, x, y FROM region WHERE id = ?" );

	SQLite::Statement query(db,  sql.c_str());

	query.bind(1, id_);

	while (query.executeStep())
	{
		id_ = query.getColumn(0);
		name_ = std::string( query.getColumn(1).getText() );
		worldId_ = query.getColumn(2);
		x_ = query.getColumn(3);
		y_ = query.getColumn(4);
	}
	
	load(ds);
}
// @deserialize_where end

// @deserialize_from_query start
void Region::deserialize(SQLite::Statement& query, pyliteserializer::SqliteDataStore& ds)
{
	id_ = query.getColumn(0);
	name_ = std::string( query.getColumn(1).getText() );
	worldId_ = query.getColumn(2);
	x_ = query.getColumn(3);
	y_ = query.getColumn(4);
	
	load(ds);
}
// @deserialize_from_query end

void Region::load(pyliteserializer::SqliteDataStore& ds)
{
}

// Testing
void Region::loadMeshes()
{
	// open file
	
	// read density values and create corresponding mesh(es)
	
	// add mesh(es) to terrain manager
	
	// close file
	
	//for ( auto& l : locales_ )
	//{
	//	l.loadMeshes();
	//}
}

void Region::tick(glmd::float32 elapsedTime)
{
	// Not sure I need to call tick for locales...
	//for (auto& l : locales_)
	//{		
	//	l.tick(elapsedTime);
	//}
}

}
}
