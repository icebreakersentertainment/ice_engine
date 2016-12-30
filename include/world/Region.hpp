#ifndef REGION_H_
#define REGION_H_

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

#include "data_store/ISqliteSerializable.hpp"

// Forward declaration
namespace pyliteserializer
{
	class SqliteDataStore;
}

#include "utilities/Macros.hpp"

namespace glmd = glm::detail;

// @include world/Region.hpp
namespace game
{
namespace world
{

// @table region
// @namespace { game::world }
class Region : public pyliteserializer::ISqliteSerializable
{
public:
	Region(int id = 0, std::string name = std::string(), int worldId = 0);
	virtual ~Region();

	// @serialize start
	virtual void serialize(SQLite::Database& db, pyliteserializer::SqliteDataStore& ds);
	// @serialize end

	// @deserialize start
	virtual void deserialize(SQLite::Database& db, pyliteserializer::SqliteDataStore& ds);
	// @deserialize end
				
	// @deserialize_where start
	virtual void deserialize(SQLite::Database& db, pyliteserializer::SqliteDataStore& ds, const std::string& where);
	// @deserialize_where end
	
	// @deserialize_from_query start
	virtual void deserialize(SQLite::Statement& query, pyliteserializer::SqliteDataStore& ds);
	// @deserialize_from_query end

	virtual void load(pyliteserializer::SqliteDataStore& ds);

	// Testing
	void loadMeshes();
	
	void tick(glmd::float32 elapsedTime);

	GETSET(int, id_, Id)
	GET(std::string, name_, Name)
	SET_CONST_REF(std::string, name_, Name)
	GETSET(int, worldId_, WorldId)
	GETSET(int, x_, X)
	GETSET(int, y_, Y)
	
private:
	// @column id
	// @id
	int id_;
	
	// @column name
	std::string name_;
	
	// @column world_id
	int worldId_;
	
	// @column x
	int x_;
	
	// @column y
	int y_;
};

}
}

#endif /* REGION_H_ */
