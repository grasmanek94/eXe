#include "../../boost_extract/boost/geometry.hpp"
#include "../../boost_extract/boost/geometry/geometries/geometries.hpp"
#include "../../boost_extract/boost/intrusive_ptr.hpp"

//#ifdef WIN32
template<typename T>
inline void intrusive_ptr_add_ref(T *t)
{
	++t->references;
}

template<typename T>
inline void intrusive_ptr_release(T *t)
{
	if (!(--t->references))
	{
		delete t;
	}
}
//#endif

#include "../../boost_extract/boost/tuple/tuple.hpp"
#include "../../boost_extract/boost/scoped_ptr.hpp"
#include "../../boost_extract/boost/unordered_map.hpp"
#include "../../boost_extract/boost/chrono.hpp"
#include "../../boost_extract/boost/tuple/tuple.hpp"
#include <limits>
#include <string>
#include <functional>
#include <queue>
#include <vector>
#include "../../boost_extract/boost/unordered_set.hpp"
#include "../../boost_extract/boost/variant.hpp"
#include <bitset>
#include <set>
#include <cmath>
#include <map>
#include "StreamerLib.hxx"
#include <algorithm>
#include <sstream>
#include <Eigen/Core>
#include "../sampgdk/sampgdk.h"
using namespace sampgdk;
#include <utility>

#define INCLUDE_FILE_VERSION (0x27401)
#define PLUGIN_VERSION "2.7.4"

#define STREAMER_MAX_TYPES (7)

#define INVALID_ALTERNATE_ID (-1)
#define INVALID_GENERIC_ID (0xFFFF)

#define STREAMER_TYPE_OBJECT (0)
#define STREAMER_TYPE_PICKUP (1)
#define STREAMER_TYPE_CP (2)
#define STREAMER_TYPE_RACE_CP (3)
#define STREAMER_TYPE_MAP_ICON (4)
#define STREAMER_TYPE_3D_TEXT_LABEL (5)
#define STREAMER_TYPE_AREA (6)

#define STREAMER_MAX_AREA_TYPES (5)

#define STREAMER_AREA_TYPE_CIRCLE (0)
#define STREAMER_AREA_TYPE_CYLINDER (1)
#define STREAMER_AREA_TYPE_SPHERE (2)
#define STREAMER_AREA_TYPE_RECTANGLE (3)
#define STREAMER_AREA_TYPE_CUBOID (4)
#define STREAMER_AREA_TYPE_POLYGON (5)

#define STREAMER_MAX_OBJECT_TYPES (3)

#define STREAMER_OBJECT_TYPE_GLOBAL (0)
#define STREAMER_OBJECT_TYPE_PLAYER (1)
#define STREAMER_OBJECT_TYPE_DYNAMIC (2)


#ifdef MAX_PLAYERS
#undef MAX_PLAYERS
#define MAX_PLAYERS (1000)
#endif

class Cell;
class Data;
class Events;
class Identifier;
class Grid;
struct Player;
class Streamer;

typedef std::pair<int, int> CellID;
typedef boost::intrusive_ptr<Cell> SharedCell;

typedef boost::geometry::model::polygon<Eigen::Vector2f> Polygon2D;

typedef boost::geometry::model::box<Eigen::Vector2f> Box2D;
typedef boost::geometry::model::box<Eigen::Vector3f> Box3D;

namespace Item
{
	struct Area;
	struct Checkpoint;
	struct MapIcon;
	struct Object;
	struct Pickup;
	struct RaceCheckpoint;
	struct TextLabel;

	typedef boost::intrusive_ptr<Area> SharedArea;
	typedef boost::intrusive_ptr<Checkpoint> SharedCheckpoint;
	typedef boost::intrusive_ptr<MapIcon> SharedMapIcon;
	typedef boost::intrusive_ptr<Object> SharedObject;
	typedef boost::intrusive_ptr<Pickup> SharedPickup;
	typedef boost::intrusive_ptr<RaceCheckpoint> SharedRaceCheckpoint;
	typedef boost::intrusive_ptr<TextLabel> SharedTextLabel;
}

namespace boost {
	namespace geometry {
		namespace traits {
			template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
			struct tag<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> >
			{
				typedef point_tag type;
			};

			template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
			struct coordinate_type<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> >
			{
				typedef _Scalar type;
			};

			template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
			struct coordinate_system<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> >
			{
				typedef cs::cartesian type;
			};

			template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
			struct dimension<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> > : boost::mpl::int_<_Rows>{};

			template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols, std::size_t Dimension>
			struct access<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>, Dimension>
			{
				static inline _Scalar get(Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> const &matrix)
				{
					return matrix[Dimension];
				}

				static inline void set(Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &matrix, _Scalar const &value)
				{
					matrix[Dimension] = value;
				}
			};
		}
	}
}

class Identifier
{
public:
	Identifier();

	int get();
	void remove(int id, std::size_t remaining);
	void reset();
private:
	int highestID;

	std::priority_queue<int, std::vector<int>, std::greater<int> > removedIDs;
};

namespace Item
{
	struct Area
	{
		Area();

		AMX *amx;
		int areaID;
		SharedCell cell;
		Eigen::Vector2f height;
		int references;
		float size;
		int type;

		boost::variant<Polygon2D, Box2D, Box3D, Eigen::Vector2f, Eigen::Vector3f> position;

		struct Attach
		{
			Attach();

			boost::tuple<int, int, int> object;
			int player;
			Eigen::Vector3f position;
			int references;
			int vehicle;

			EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		};

		boost::intrusive_ptr<Attach> attach;

		std::vector<int> extras;
		boost::unordered_set<int> interiors;
		std::bitset<MAX_PLAYERS> players;
		boost::unordered_set<int> worlds;

		static Identifier identifier;

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	struct Checkpoint
	{
		Checkpoint();

		AMX *amx;
		SharedCell cell;
		int checkpointID;
		Eigen::Vector3f position;
		int references;
		float size;
		float streamDistance;

		std::vector<int> extras;
		boost::unordered_set<int> interiors;
		std::bitset<MAX_PLAYERS> players;
		boost::unordered_set<int> worlds;

		static Identifier identifier;

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	struct MapIcon
	{
		MapIcon();

		AMX *amx;
		SharedCell cell;
		int color;
		int mapIconID;
		Eigen::Vector3f position;
		int references;
		float streamDistance;
		int style;
		int type;

		std::vector<int> extras;
		boost::unordered_set<int> interiors;
		std::bitset<MAX_PLAYERS> players;
		boost::unordered_set<int> worlds;

		static Identifier identifier;

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	struct Object
	{
		Object();

		AMX *amx;
		SharedCell cell;
		float drawDistance;
		int modelID;
		int objectID;
		Eigen::Vector3f position;
		int references;
		Eigen::Vector3f rotation;
		float streamDistance;

		struct Attach
		{
			Attach();

			Eigen::Vector3f position;
			Eigen::Vector3f offset;
			int references;
			Eigen::Vector3f rotation;
			int vehicle;

			EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		};

		boost::intrusive_ptr<Attach> attach;

		struct Material
		{
			struct Main
			{
				Main();

				int materialColor;
				int modelID;
				int references;
				std::string textureName;
				std::string txdFileName;
			};

			boost::intrusive_ptr<Main> main;

			struct Text
			{
				Text();

				int backColor;
				bool bold;
				int fontColor;
				std::string fontFace;
				int fontSize;
				int references;
				int materialSize;
				std::string materialText;
				int textAlignment;
			};

			boost::intrusive_ptr<Text> text;
		};

		boost::unordered_map<int, Material> materials;

		struct Move
		{
			Move();

			int duration;
			boost::tuple<Eigen::Vector3f, Eigen::Vector3f, Eigen::Vector3f> position;
			int references;
			boost::tuple<Eigen::Vector3f, Eigen::Vector3f, Eigen::Vector3f> rotation;
			float speed;
			boost::chrono::steady_clock::time_point time;

			EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		};

		boost::intrusive_ptr<Move> move;

		std::vector<int> extras;
		boost::unordered_set<int> interiors;
		std::bitset<MAX_PLAYERS> players;
		boost::unordered_set<int> worlds;

		static Identifier identifier;

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	struct Pickup
	{
		Pickup();

		AMX *amx;
		SharedCell cell;
		int modelID;
		int pickupID;
		Eigen::Vector3f position;
		int references;
		float streamDistance;
		int type;
		int worldID;

		std::vector<int> extras;
		boost::unordered_set<int> interiors;
		std::bitset<MAX_PLAYERS> players;
		boost::unordered_set<int> worlds;

		static Identifier identifier;

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	struct RaceCheckpoint
	{
		RaceCheckpoint();

		AMX *amx;
		SharedCell cell;
		Eigen::Vector3f next;
		Eigen::Vector3f position;
		int raceCheckpointID;
		int references;
		float size;
		float streamDistance;
		int type;

		std::vector<int> extras;
		boost::unordered_set<int> interiors;
		std::bitset<MAX_PLAYERS> players;
		boost::unordered_set<int> worlds;

		static Identifier identifier;

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	struct TextLabel
	{
		TextLabel();

		AMX *amx;
		SharedCell cell;
		int color;
		float drawDistance;
		Eigen::Vector3f position;
		int references;
		float streamDistance;
		bool testLOS;
		std::string text;
		int textLabelID;

		struct Attach
		{
			Attach();

			int player;
			Eigen::Vector3f position;
			int references;
			int vehicle;
		};

		boost::intrusive_ptr<Attach> attach;

		std::vector<int> extras;
		boost::unordered_set<int> interiors;
		std::bitset<MAX_PLAYERS> players;
		boost::unordered_set<int> worlds;

		static Identifier identifier;

		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};
}

class Cell
{
public:
	Cell();
	Cell(CellID cellID);

	CellID cellID;
	int references;

	boost::unordered_map<int, Item::SharedArea> areas;
	boost::unordered_map<int, Item::SharedCheckpoint> checkpoints;
	boost::unordered_map<int, Item::SharedMapIcon> mapIcons;
	boost::unordered_map<int, Item::SharedObject> objects;
	boost::unordered_map<int, Item::SharedPickup> pickups;
	boost::unordered_map<int, Item::SharedRaceCheckpoint> raceCheckpoints;
	boost::unordered_map<int, Item::SharedTextLabel> textLabels;
};

struct Player
{
	Player(int playerID);

	int activeCheckpoint;
	int activeRaceCheckpoint;
	int interiorID;
	int playerID;
	Eigen::Vector3f position;
	int references;
	bool updateWhenIdle;
	SharedCell visibleCell;
	int visibleCheckpoint;
	int visibleRaceCheckpoint;
	std::size_t visibleObjects;
	std::size_t visibleTextLabels;
	int worldID;

	boost::unordered_set<int> disabledAreas;
	boost::unordered_set<int> disabledCheckpoints;
	boost::unordered_set<int> disabledRaceCheckpoints;

	std::bitset<STREAMER_MAX_TYPES> enabledItems;

	boost::unordered_set<int> internalAreas;
	boost::unordered_map<int, int> internalMapIcons;
	boost::unordered_map<int, int> internalObjects;
	boost::unordered_map<int, int> internalTextLabels;

	Identifier mapIconIdentifier;

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class Data
{
public:
	Data();

	std::size_t getMaxItems(int type);
	bool setMaxItems(int type, std::size_t value);

	std::set<AMX*> interfaces;

	boost::unordered_map<int, Item::SharedArea> areas;
	boost::unordered_map<int, Item::SharedCheckpoint> checkpoints;
	boost::unordered_map<int, Item::SharedMapIcon> mapIcons;
	boost::unordered_map<int, Item::SharedObject> objects;
	boost::unordered_map<int, Item::SharedPickup> pickups;
	boost::unordered_map<int, Item::SharedRaceCheckpoint> raceCheckpoints;
	boost::unordered_map<int, Item::SharedTextLabel> textLabels;

	boost::unordered_map<int, Player> players;
private:
	std::size_t maxAreas;
	std::size_t maxCheckpoints;
	std::size_t maxMapIcons;
	std::size_t maxObjects;
	std::size_t maxPickups;
	std::size_t maxRaceCheckpoints;
	std::size_t maxTextLabels;
};

class Grid
{
public:
	Grid();

	void addArea(const Item::SharedArea &area);
	void addCheckpoint(const Item::SharedCheckpoint &checkpoint);
	void addMapIcon(const Item::SharedMapIcon &mapIcon);
	void addObject(const Item::SharedObject &object);
	void addPickup(const Item::SharedPickup &pickup);
	void addRaceCheckpoint(const Item::SharedRaceCheckpoint &raceCheckpoint);
	void addTextLabel(const Item::SharedTextLabel &textLabel);

	inline float getCellSize()
	{
		return cellSize;
	}

	inline float getCellDistance()
	{
		return sqrt(cellDistance);
	}

	inline void setCellSize(float size)
	{
		cellSize = size;
	}

	inline void setCellDistance(float distance)
	{
		cellDistance = distance * distance;
	}

	void rebuildGrid();

	void removeArea(const Item::SharedArea &area, bool reassign = false);
	void removeCheckpoint(const Item::SharedCheckpoint &checkpoint, bool reassign = false);
	void removeMapIcon(const Item::SharedMapIcon &mapIcon, bool reassign = false);
	void removeObject(const Item::SharedObject &object, bool reassign = false);
	void removePickup(const Item::SharedPickup &pickup, bool reassign = false);
	void removeRaceCheckpoint(const Item::SharedRaceCheckpoint &raceCheckpoint, bool reassign = false);
	void removeTextLabel(const Item::SharedTextLabel &textLabel, bool reassign = false);

	void findAllCells(Player &player, std::vector<SharedCell> &playerCells);
	void findMinimalCells(Player &player, std::vector<SharedCell> &playerCells);
private:
	float cellDistance;
	float cellSize;
	SharedCell globalCell;
	Eigen::Matrix<float, 2, 9> translationMatrix;

	boost::unordered_map<CellID, SharedCell> cells;

	inline void calculateTranslationMatrix()
	{
		translationMatrix << 0.0f, 0.0f, cellSize, cellSize, cellSize * -1.0f, 0.0f, cellSize * -1.0f, cellSize, cellSize * -1.0f,
			0.0f, cellSize, 0.0f, cellSize, 0.0f, cellSize * -1.0f, cellSize, cellSize * -1.0f, cellSize * -1.0f;
	}

	inline void eraseCellIfEmpty(const SharedCell &cell)
	{
		if (cell->areas.empty() && cell->checkpoints.empty() && cell->mapIcons.empty() && cell->objects.empty() && cell->pickups.empty() && cell->raceCheckpoints.empty() && cell->textLabels.empty())
		{
			cells.erase(cell->cellID);
		}
	}

	CellID getCellID(const Eigen::Vector2f &position, bool insert = true);
	void processDiscoveredCells(Player &player, std::vector<SharedCell> &playerCells, const boost::unordered_set<CellID> &discoveredCells);
};

class Streamer
{
public:
	Streamer();

	inline std::size_t getTickRate()
	{
		return tickRate;
	}

	inline void setTickRate(std::size_t value)
	{
		tickRate = value;
	}

	std::size_t getVisibleItems(int type);
	bool setVisibleItems(int type, std::size_t value);

	void startAutomaticUpdate();
	void startManualUpdate(Player &player, bool getData);

	void processActiveItems();

	boost::unordered_set<Item::SharedArea> attachedAreas;
	boost::unordered_set<Item::SharedObject> attachedObjects;
	boost::unordered_set<Item::SharedTextLabel> attachedTextLabels;
	boost::unordered_set<Item::SharedObject> movingObjects;

	boost::unordered_map<int, int> internalPickups;

	std::vector<boost::tuple<int, int> > areaEnterCallbacks;
	std::vector<boost::tuple<int, int> > areaLeaveCallbacks;
private:
	void calculateAverageUpdateTime();

	void performPlayerUpdate(Player &player, bool automatic);
	void executeCallbacks();

	void processAreas(Player &player, const std::vector<SharedCell> &cells);
	void processCheckpoints(Player &player, const std::vector<SharedCell> &cells);
	void processMapIcons(Player &player, const std::vector<SharedCell> &cells);
	void processObjects(Player &player, const std::vector<SharedCell> &cells);
	void processPickups(Player &player, const std::vector<SharedCell> &cells);
	void processRaceCheckpoints(Player &player, const std::vector<SharedCell> &cells);
	void processTextLabels(Player &player, const std::vector<SharedCell> &cells);

	void processMovingObjects();
	void processAttachedAreas();
	void processAttachedObjects();
	void processAttachedTextLabels();

	std::size_t tickCount;
	std::size_t tickRate;

	boost::tuple<float, float> velocityBoundaries;

	std::size_t visibleMapIcons;
	std::size_t visibleObjects;
	std::size_t visiblePickups;
	std::size_t visibleTextLabels;

	float averageUpdateTime;
	bool processingFinalPlayer;

	std::vector<int> objectMoveCallbacks;

	template<std::size_t N, typename T>
	inline bool checkPlayer(const std::bitset<N> &a, const T &b, const boost::unordered_set<T> &c, const T &d, const boost::unordered_set<T> &e, const T &f)
	{
		return (a[b] && (c.empty() || c.find(d) != c.end()) && (e.empty() || e.find(f) != e.end()));
	}

	template<class Iterator, class Container>
	bool isLastPlayer(Iterator iterator, const Container &container)
	{
		return ((iterator != container.end()) && (++iterator == container.end()));
	}
};

class Core
{
public:
	Core();

	inline Data *getData()
	{
		return data.get();
	}

	inline Grid *getGrid()
	{
		return grid.get();
	}

	inline Streamer *getStreamer()
	{
		return streamer.get();
	}
private:
	boost::scoped_ptr<Data> data;
	boost::scoped_ptr<Grid> grid;
	boost::scoped_ptr<Streamer> streamer;
};

extern boost::scoped_ptr<Core> core;

#define STREAMER_OPC (0)
#define STREAMER_OPDC (1)
#define STREAMER_OPEO (2)
#define STREAMER_OPSO (3)
#define STREAMER_OPPP (4)
#define STREAMER_OPEC (5)
#define STREAMER_OPLC (6)
#define STREAMER_OPERC (7)
#define STREAMER_OPLRC (8)
#define STREAMER_OPWS (9)


#define CHECK_PARAMS(m, n)


namespace Utility
{
	cell AMX_NATIVE_CALL hookedNative(AMX *amx, cell *params);

	int checkInterfaceAndRegisterNatives(AMX *amx, AMX_NATIVE_INFO *amxNativeList);
	void destroyAllItemsInInterface(AMX *amx);

	boost::unordered_map<int, Item::SharedArea>::iterator destroyArea(boost::unordered_map<int, Item::SharedArea>::iterator a);
	boost::unordered_map<int, Item::SharedCheckpoint>::iterator destroyCheckpoint(boost::unordered_map<int, Item::SharedCheckpoint>::iterator c);
	boost::unordered_map<int, Item::SharedMapIcon>::iterator destroyMapIcon(boost::unordered_map<int, Item::SharedMapIcon>::iterator m);
	boost::unordered_map<int, Item::SharedObject>::iterator destroyObject(boost::unordered_map<int, Item::SharedObject>::iterator o);
	boost::unordered_map<int, Item::SharedPickup>::iterator destroyPickup(boost::unordered_map<int, Item::SharedPickup>::iterator p);
	boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator destroyRaceCheckpoint(boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r);
	boost::unordered_map<int, Item::SharedTextLabel>::iterator destroyTextLabel(boost::unordered_map<int, Item::SharedTextLabel>::iterator t);

	bool isPointInArea(const Eigen::Vector3f &point, const Item::SharedArea &area);

	template<typename T>
	inline bool addToContainer(std::vector<T> &container, T value)
	{
		container.push_back(value);
		return true;
	}

	template<typename T>
	inline bool addToContainer(boost::unordered_set<T> &container, T value)
	{
		if (value >= 0)
		{
			container.insert(value);
			return true;
		}
		else
		{
			container.clear();
		}
		return false;
	}

	template<std::size_t N, typename T>
	inline bool addToContainer(std::bitset<N> &container, T value)
	{
		if (value >= 0 && static_cast<std::size_t>(value) < N)
		{
			container.set(static_cast<std::size_t>(value));
			return true;
		}
		else
		{
			container.set();
		}
		return false;
	}

	template<typename T>
	inline int getFirstValueInContainer(std::vector<T> &container)
	{
		if (!container.empty())
		{
			return container.front();
		}
		return 0;
	}

	template<typename T>
	inline int getFirstValueInContainer(boost::unordered_set<T> &container, int defaultret = -1)
	{
		boost::unordered_set<int>::iterator i = container.begin();
		if (i != container.end())
		{
			return *i;
		}
		return defaultret;
	}

	template<std::size_t N>
	inline int getFirstValueInContainer(std::bitset<N> &container)
	{
		if (container.any())
		{
			for (std::size_t i = 0; i < N; ++i)
			{
				if (container.test(i))
				{
					return i;
				}
			}
		}
		return -1;
	}

	template<typename T>
	inline bool setFirstValueInContainer(std::vector<T> &container, T value)
	{
		container.clear();
		return addToContainer(container, value);
	}

	template<typename T>
	inline bool setFirstValueInContainer(boost::unordered_set<T> &container, T value)
	{
		container.clear();
		return addToContainer(container, value);
	}

	template<std::size_t N, typename T>
	inline bool setFirstValueInContainer(std::bitset<N> &container, T value)
	{
		container.reset();
		return addToContainer(container, value);
	}

	template<typename T>
	inline bool isInContainer(std::vector<T> &container, T value)
	{
		if (std::find(container.begin(), container.end(), value) != container.end())
		{
			return true;
		}
		return false;
	}

	template<typename T>
	inline bool isInContainer(boost::unordered_set<T> &container, T value)
	{
		if (value >= 0)
		{
			if (container.find(value) != container.end())
			{
				return true;
			}
		}
		else
		{
			if (container.empty())
			{
				return true;
			}
		}
		return false;
	}

	template<std::size_t N, typename T>
	inline bool isInContainer(std::bitset<N> &container, T value)
	{
		if (value >= 0 && static_cast<std::size_t>(value) < N)
		{
			if (container[static_cast<std::size_t>(value)])
			{
				return true;
			}
		}
		else
		{
			if (container.count() == N)
			{
				return true;
			}
		}
		return false;
	}

	template<typename T>
	inline bool removeFromContainer(std::vector<T> &container, T value)
	{
		typename std::vector<T>::iterator i = std::find(container.begin(), container.end(), value);
		if (i != container.end())
		{
			container.erase(i);
			return true;
		}
		return false;
	}

	template<typename T>
	inline bool removeFromContainer(boost::unordered_set<T> &container, T value)
	{
		if (value >= 0)
		{
			container.erase(value);
			return true;
		}
		else
		{
			container.clear();
		}
		return false;
	}

	template<std::size_t N, typename T>
	inline bool removeFromContainer(std::bitset<N> &container, T value)
	{
		if (value >= 0 && static_cast<std::size_t>(value) < N)
		{
			container.reset(static_cast<std::size_t>(value));
			return true;
		}
		else
		{
			container.reset();
		}
		return false;
	}

	template<typename T>
	inline bool convertArrayToContainer(AMX *amx, cell input, cell size, std::vector<T> &container)
	{
		cell *array = NULL;
		amx_GetAddr(amx, input, &array);
		container.clear();
		for (std::size_t i = 0; i < static_cast<std::size_t>(size); ++i)
		{
			if (!addToContainer(container, static_cast<T>(array[i])))
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	inline bool convertArrayToContainer(AMX *amx, cell input, cell size, boost::unordered_set<T> &container)
	{
		cell *array = NULL;
		amx_GetAddr(amx, input, &array);
		container.clear();
		for (std::size_t i = 0; i < static_cast<std::size_t>(size); ++i)
		{
			if (!addToContainer(container, static_cast<T>(array[i])))
			{
				return false;
			}
		}
		return true;
	}

	template<std::size_t N>
	inline bool convertArrayToContainer(AMX *amx, cell input, cell size, std::bitset<N> &container)
	{
		cell *array = NULL;
		amx_GetAddr(amx, input, &array);
		container.reset();
		for (std::size_t i = 0; i < static_cast<std::size_t>(size); ++i)
		{
			if (!addToContainer(container, static_cast<std::size_t>(array[i])))
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	inline bool convertContainerToArray(AMX *amx, cell output, cell size, std::vector<T> &container)
	{
		cell *array = NULL;
		std::size_t i = 0;
		amx_GetAddr(amx, output, &array);
		for (typename std::vector<T>::iterator c = container.begin(); c != container.end(); ++c)
		{
			if (i == static_cast<std::size_t>(size))
			{
				return false;
			}
			array[i++] = static_cast<cell>(*c);
		}
		return true;
	}

	template<typename T>
	inline bool convertContainerToArray(AMX *amx, cell output, cell size, boost::unordered_set<T> &container)
	{
		cell *array = NULL;
		std::size_t i = 0;
		amx_GetAddr(amx, output, &array);
		for (typename boost::unordered_set<T>::iterator c = container.begin(); c != container.end(); ++c)
		{
			if (i == static_cast<std::size_t>(size))
			{
				return false;
			}
			array[i++] = static_cast<cell>(*c);
		}
		return true;
	}

	template<std::size_t N>
	inline bool convertContainerToArray(AMX *amx, cell output, cell size, std::bitset<N> &container)
	{
		cell *array = NULL;
		std::size_t i = 0;
		amx_GetAddr(amx, output, &array);
		for (std::size_t c = 0; c < N; ++c)
		{
			if (i == static_cast<std::size_t>(size))
			{
				return false;
			}
			if (container[c])
			{
				array[i++] = static_cast<cell>(c);
			}
		}
		return true;
	}

	void convertArrayToPolygon(AMX *amx, cell input, cell size, Polygon2D &polygon);
	bool convertPolygonToArray(AMX *amx, cell output, cell size, Polygon2D &polygon);
	std::string convertNativeStringToString(AMX *amx, cell input);
	void convertStringToNativeString(AMX *amx, cell output, cell size, std::string string);
	void storeFloatInNative(AMX *amx, cell output, float number);
	void storeIntegerInNative(AMX *amx, cell output, int number);
}

namespace Manipulation
{
	enum
	{
		AttachedObject,
		AttachedPlayer,
		AttachedVehicle,
		AttachOffsetX,
		AttachOffsetY,
		AttachOffsetZ,
		AttachRX,
		AttachRY,
		AttachRZ,
		AttachX,
		AttachY,
		AttachZ,
		Color,
		DrawDistance,
		ExtraID,
		InteriorID,
		MaxX,
		MaxY,
		MaxZ,
		MinX,
		MinY,
		MinZ,
		ModelID,
		MoveRX,
		MoveRY,
		MoveRZ,
		MoveSpeed,
		MoveX,
		MoveY,
		MoveZ,
		NextX,
		NextY,
		NextZ,
		PlayerID,
		RX,
		RY,
		RZ,
		Size,
		StreamDistance,
		Style,
		TestLOS,
		Type,
		WorldID,
		X,
		Y,
		Z
	};

	enum
	{
		InvalidData,
		InvalidID,
		InvalidType
	};

	int getFloatData(AMX *amx, cell *params);
	int setFloatData(AMX *amx, cell *params);
	int getIntData(AMX *amx, cell *params);
	int setIntData(AMX *amx, cell *params);
	int getArrayData(AMX *amx, cell *params);
	int setArrayData(AMX *amx, cell *params);
	int isInArrayData(AMX *amx, cell *params);
	int appendArrayData(AMX *amx, cell *params);
	int removeArrayData(AMX *amx, cell *params);

	template <typename T>
	int getArrayDataForItem(T &container, AMX *amx, int id, int type, cell output, cell size, int &error)
	{
		typename T::iterator i = container.find(id);
		if (i != container.end())
		{
			switch (type)
			{
			case ExtraID:
			{
				return Utility::convertContainerToArray(amx, output, size, i->second->extras) != 0;
			}
			case InteriorID:
			{
				return Utility::convertContainerToArray(amx, output, size, i->second->interiors) != 0;
			}
			case PlayerID:
			{
				return Utility::convertContainerToArray(amx, output, size, i->second->players) != 0;
			}
			case WorldID:
			{
				return Utility::convertContainerToArray(amx, output, size, i->second->worlds) != 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		return 0;
	}

	template <typename T>
	int setArrayDataForItem(T &container, AMX *amx, int id, int type, cell input, cell size, int &error)
	{
		typename T::iterator i = container.find(id);
		if (i != container.end())
		{
			switch (type)
			{
			case ExtraID:
			{
				return Utility::convertArrayToContainer(amx, input, size, i->second->extras) != 0;
			}
			case InteriorID:
			{
				return Utility::convertArrayToContainer(amx, input, size, i->second->interiors) != 0;
			}
			case PlayerID:
			{
				return Utility::convertArrayToContainer(amx, input, size, i->second->players) != 0;
			}
			case WorldID:
			{
				return Utility::convertArrayToContainer(amx, input, size, i->second->worlds) != 0;
			}
			default:
			{
				error = InvalidData;
				return 0;
			}
			}
		}
		error = InvalidID;
		return 0;
	}

	template <typename T>
	int isInArrayDataForItem(T &container, int id, int type, int value, int &error)
	{
		typename T::iterator i = container.find(id);
		if (i != container.end())
		{
			switch (type)
			{
			case ExtraID:
			{
				return Utility::isInContainer(i->second->extras, value) != 0;
			}
			case InteriorID:
			{
				return Utility::isInContainer(i->second->interiors, value) != 0;
			}
			case PlayerID:
			{
				return Utility::isInContainer(i->second->players, value) != 0;
			}
			case WorldID:
			{
				return Utility::isInContainer(i->second->worlds, value) != 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		return 0;
	}

	template <typename T>
	int appendArrayDataForItem(T &container, int id, int type, int value, int &error)
	{
		typename T::iterator i = container.find(id);
		if (i != container.end())
		{
			switch (type)
			{
			case ExtraID:
			{
				return Utility::addToContainer(i->second->extras, value) != 0;
			}
			case InteriorID:
			{
				return Utility::addToContainer(i->second->interiors, value) != 0;
			}
			case PlayerID:
			{
				return Utility::addToContainer(i->second->players, value) != 0;
			}
			case WorldID:
			{
				return Utility::addToContainer(i->second->worlds, value) != 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		return 0;
	}

	template <typename T>
	int removeArrayDataForItem(T &container, int id, int type, int value, int &error)
	{
		typename T::iterator i = container.find(id);
		if (i != container.end())
		{
			switch (type)
			{
			case ExtraID:
			{
				return Utility::removeFromContainer(i->second->extras, value) != 0;
			}
			case InteriorID:
			{
				return Utility::removeFromContainer(i->second->interiors, value) != 0;
			}
			case PlayerID:
			{
				return Utility::removeFromContainer(i->second->players, value) != 0;
			}
			case WorldID:
			{
				return Utility::removeFromContainer(i->second->worlds, value) != 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		return 0;
	}
}

extern AMX StreamerAMX;

extern void *pAMXFunctions;

namespace StreamerLibrary
{
	void Load()
	{
		core.reset(new Core);
		//sampgdk::logprintf("\n\n*** Streamer Plugin v%s by Incognito loaded ***\n", PLUGIN_VERSION);
	}

	void Unload()
	{
		core.reset();
		//sampgdk::logprintf("\n\n*** Streamer Plugin v%s by Incognito unloaded ***\n", PLUGIN_VERSION);
	}

	void ProcessTick()
	{
		core->getStreamer()->startAutomaticUpdate();
	}
};

using namespace Manipulation;

int Manipulation::getFloatData(AMX *amx, cell *params)
{
	int error = -1;
	switch (static_cast<int>(params[1]))
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[2]));
		if (o != core->getData()->objects.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case AttachOffsetX:
			{
				if (o->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->attach->offset[0]);
					return 1;
				}
				return 0;
			}
			case AttachOffsetY:
			{
				if (o->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->attach->offset[1]);
					return 1;
				}
				return 0;
			}
			case AttachOffsetZ:
			{
				if (o->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->attach->offset[2]);
					return 1;
				}
				return 0;
			}
			case AttachRX:
			{
				if (o->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->attach->rotation[0]);
					return 1;
				}
				return 0;
			}
			case AttachRY:
			{
				if (o->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->attach->rotation[1]);
					return 1;
				}
				return 0;
			}
			case AttachRZ:
			{
				if (o->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->attach->rotation[2]);
					return 1;
				}
				return 0;
			}
			case AttachX:
			{
				if (o->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->attach->position[0]);
					return 1;
				}
				return 0;
			}
			case AttachY:
			{
				if (o->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->attach->position[1]);
					return 1;
				}
				return 0;
			}
			case AttachZ:
			{
				if (o->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->attach->position[2]);
					return 1;
				}
				return 0;
			}
			case DrawDistance:
			{
				Utility::storeFloatInNative(amx, params[4], o->second->drawDistance);
				return 1;
			}
			case MoveSpeed:
			{
				if (o->second->move)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->move->speed);
					return 1;
				}
				return 0;
			}
			case MoveRX:
			{
				if (o->second->move)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->move->rotation.get<0>()[0]);
					return 1;
				}
				return 0;
			}
			case MoveRY:
			{
				if (o->second->move)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->move->rotation.get<0>()[1]);
					return 1;
				}
				return 0;
			}
			case MoveRZ:
			{
				if (o->second->move)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->move->rotation.get<0>()[2]);
					return 1;
				}
				return 0;
			}
			case MoveX:
			{
				if (o->second->move)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->move->position.get<0>()[0]);
					return 1;
				}
				return 0;
			}
			case MoveY:
			{
				if (o->second->move)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->move->position.get<0>()[1]);
					return 1;
				}
				return 0;
			}
			case MoveZ:
			{
				if (o->second->move)
				{
					Utility::storeFloatInNative(amx, params[4], o->second->move->position.get<0>()[2]);
					return 1;
				}
				return 0;
			}
			case RX:
			{
				Utility::storeFloatInNative(amx, params[4], o->second->rotation[0]);
				return 1;
			}
			case RY:
			{
				Utility::storeFloatInNative(amx, params[4], o->second->rotation[1]);
				return 1;
			}
			case RZ:
			{
				Utility::storeFloatInNative(amx, params[4], o->second->rotation[2]);
				return 1;
			}
			case StreamDistance:
			{
				Utility::storeFloatInNative(amx, params[4], std::sqrt(o->second->streamDistance));
				return 1;
			}
			case X:
			{
				Utility::storeFloatInNative(amx, params[4], o->second->position[0]);
				return 1;
			}
			case Y:
			{
				Utility::storeFloatInNative(amx, params[4], o->second->position[1]);
				return 1;
			}
			case Z:
			{
				Utility::storeFloatInNative(amx, params[4], o->second->position[2]);
				return 1;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(static_cast<int>(params[2]));
		if (p != core->getData()->pickups.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case StreamDistance:
			{
				Utility::storeFloatInNative(amx, params[4], std::sqrt(p->second->streamDistance));
				return 1;
			}
			case X:
			{
				Utility::storeFloatInNative(amx, params[4], p->second->position[0]);
				return 1;
			}
			case Y:
			{
				Utility::storeFloatInNative(amx, params[4], p->second->position[1]);
				return 1;
			}
			case Z:
			{
				Utility::storeFloatInNative(amx, params[4], p->second->position[2]);
				return 1;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(static_cast<int>(params[2]));
		if (c != core->getData()->checkpoints.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case Size:
			{
				Utility::storeFloatInNative(amx, params[4], c->second->size);
				return 1;
			}
			case StreamDistance:
			{
				Utility::storeFloatInNative(amx, params[4], std::sqrt(c->second->streamDistance));
				return 1;
			}
			case X:
			{
				Utility::storeFloatInNative(amx, params[4], c->second->position[0]);
				return 1;
			}
			case Y:
			{
				Utility::storeFloatInNative(amx, params[4], c->second->position[1]);
				return 1;
			}
			case Z:
			{
				Utility::storeFloatInNative(amx, params[4], c->second->position[2]);
				return 1;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(static_cast<int>(params[2]));
		if (r != core->getData()->raceCheckpoints.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case NextX:
			{
				Utility::storeFloatInNative(amx, params[4], r->second->next[0]);
				return 1;
			}
			case NextY:
			{
				Utility::storeFloatInNative(amx, params[4], r->second->next[1]);
				return 1;
			}
			case NextZ:
			{
				Utility::storeFloatInNative(amx, params[4], r->second->next[2]);
				return 1;
			}
			case Size:
			{
				Utility::storeFloatInNative(amx, params[4], r->second->size);
				return 1;
			}
			case StreamDistance:
			{
				Utility::storeFloatInNative(amx, params[4], std::sqrt(r->second->streamDistance));
				return 1;
			}
			case X:
			{
				Utility::storeFloatInNative(amx, params[4], r->second->position[0]);
				return 1;
			}
			case Y:
			{
				Utility::storeFloatInNative(amx, params[4], r->second->position[1]);
				return 1;
			}
			case Z:
			{
				Utility::storeFloatInNative(amx, params[4], r->second->position[2]);
				return 1;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(static_cast<int>(params[2]));
		if (m != core->getData()->mapIcons.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case StreamDistance:
			{
				Utility::storeFloatInNative(amx, params[4], std::sqrt(m->second->streamDistance));
				return 1;
			}
			case X:
			{
				Utility::storeFloatInNative(amx, params[4], m->second->position[0]);
				return 1;
			}
			case Y:
			{
				Utility::storeFloatInNative(amx, params[4], m->second->position[1]);
				return 1;
			}
			case Z:
			{
				Utility::storeFloatInNative(amx, params[4], m->second->position[2]);
				return 1;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(static_cast<int>(params[2]));
		if (t != core->getData()->textLabels.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case AttachX:
			{
				if (t->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], t->second->attach->position[0]);
					return 1;
				}
				return 0;
			}
			case AttachY:
			{
				if (t->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], t->second->attach->position[1]);
					return 1;
				}
				return 0;
			}
			case AttachZ:
			{
				if (t->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], t->second->attach->position[2]);
					return 1;
				}
				return 0;
			}
			case DrawDistance:
			{
				Utility::storeFloatInNative(amx, params[4], t->second->drawDistance);
				return 1;
			}
			case StreamDistance:
			{
				Utility::storeFloatInNative(amx, params[4], std::sqrt(t->second->streamDistance));
				return 1;
			}
			case AttachOffsetX:
			case X:
			{
				Utility::storeFloatInNative(amx, params[4], t->second->position[0]);
				return 1;
			}
			case AttachOffsetY:
			case Y:
			{
				Utility::storeFloatInNative(amx, params[4], t->second->position[1]);
				return 1;
			}
			case AttachOffsetZ:
			case Z:
			{
				Utility::storeFloatInNative(amx, params[4], t->second->position[2]);
				return 1;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_AREA:
	{
		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[2]));
		if (a != core->getData()->areas.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case AttachX:
			{
				if (a->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], a->second->attach->position[0]);
					return 1;
				}
				return 0;
			}
			case AttachY:
			{
				if (a->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], a->second->attach->position[1]);
					return 1;
				}
				return 0;
			}
			case AttachZ:
			{
				if (a->second->attach)
				{
					Utility::storeFloatInNative(amx, params[4], a->second->attach->position[2]);
					return 1;
				}
				return 0;
			}
			case MaxX:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_RECTANGLE:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Box2D>(a->second->position).max_corner()[0]);
					return 1;
				}
				case STREAMER_AREA_TYPE_CUBOID:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Box3D>(a->second->position).max_corner()[0]);
					return 1;
				}
				}
				return 0;
			}
			case MaxY:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_RECTANGLE:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Box2D>(a->second->position).max_corner()[1]);
					return 1;
				}
				case STREAMER_AREA_TYPE_CUBOID:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Box3D>(a->second->position).max_corner()[1]);
					return 1;
				}
				}
				return 0;
			}
			case MaxZ:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_CUBOID:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Box3D>(a->second->position).max_corner()[2]);
					return 1;
				}
				case STREAMER_AREA_TYPE_CYLINDER:
				case STREAMER_AREA_TYPE_POLYGON:
				{
					Utility::storeFloatInNative(amx, params[4], a->second->height[1]);
					return 1;
				}
				}
				return 0;
			}
			case MinX:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_RECTANGLE:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Box2D>(a->second->position).min_corner()[0]);
					return 1;
				}
				case STREAMER_AREA_TYPE_CUBOID:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Box3D>(a->second->position).min_corner()[0]);
					return 1;
				}
				}
				return 0;
			}
			case MinY:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_RECTANGLE:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Box2D>(a->second->position).min_corner()[1]);
					return 1;
				}
				case STREAMER_AREA_TYPE_CUBOID:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Box3D>(a->second->position).min_corner()[1]);
					return 1;
				}
				}
				return 0;
			}
			case MinZ:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_CUBOID:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Box3D>(a->second->position).min_corner()[2]);
					return 1;
				}
				case STREAMER_AREA_TYPE_CYLINDER:
				case STREAMER_AREA_TYPE_POLYGON:
				{
					Utility::storeFloatInNative(amx, params[4], a->second->height[0]);
					return 1;
				}
				}
				return 0;
			}
			case Size:
			{
				Utility::storeFloatInNative(amx, params[4], std::sqrt(a->second->size));
				return 1;
			}
			case X:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_CIRCLE:
				case STREAMER_AREA_TYPE_CYLINDER:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Eigen::Vector2f>(a->second->position)[0]);
					return 1;
				}
				case STREAMER_AREA_TYPE_SPHERE:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Eigen::Vector3f>(a->second->position)[0]);
					return 1;
				}
				}
				return 0;
			}
			case Y:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_CIRCLE:
				case STREAMER_AREA_TYPE_CYLINDER:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Eigen::Vector2f>(a->second->position)[1]);
					return 1;
				}
				case STREAMER_AREA_TYPE_SPHERE:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Eigen::Vector3f>(a->second->position)[1]);
					return 1;
				}
				}
				return 0;
			}
			case Z:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_SPHERE:
				{
					Utility::storeFloatInNative(amx, params[4], boost::get<Eigen::Vector3f>(a->second->position)[2]);
					return 1;
				}
				}
				return 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	default:
	{
		error = InvalidType;
		break;
	}
	}
	switch (error)
	{
	case InvalidData:
	{
		sampgdk::logprintf("*** Streamer_GetFloatData: Invalid data specified");
		break;
	}
	case InvalidID:
	{
		sampgdk::logprintf("*** Streamer_GetFloatData: Invalid ID specified");
		break;
	}
	case InvalidType:
	{
		sampgdk::logprintf("*** Streamer_GetFloatData: Invalid type specified");
		break;
	}
	}
	return 0;
}

int Manipulation::setFloatData(AMX *amx, cell *params)
{
	int error = -1;
	bool reassign = false, update = false;
	switch (static_cast<int>(params[1]))
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[2]));
		if (o != core->getData()->objects.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case AttachOffsetX:
			{
				if (o->second->attach)
				{
					o->second->attach->offset[0] = amx_ctof(params[4]);
					update = true;
				}
				break;
			}
			case AttachOffsetY:
			{
				if (o->second->attach)
				{
					o->second->attach->offset[1] = amx_ctof(params[4]);
					update = true;
				}
				break;
			}
			case AttachOffsetZ:
			{
				if (o->second->attach)
				{
					o->second->attach->offset[2] = amx_ctof(params[4]);
					update = true;
				}
				break;
			}
			case AttachRX:
			{
				if (o->second->attach)
				{
					o->second->attach->rotation[0] = amx_ctof(params[4]);
					update = true;
				}
				break;
			}
			case AttachRY:
			{
				if (o->second->attach)
				{
					o->second->attach->rotation[1] = amx_ctof(params[4]);
					update = true;
				}
				break;
			}
			case AttachRZ:
			{
				if (o->second->attach)
				{
					o->second->attach->rotation[2] = amx_ctof(params[4]);
					update = true;
				}
				break;
			}
			case DrawDistance:
			{
				o->second->drawDistance = amx_ctof(params[4]);
				update = true;
				break;
			}
			case MoveRX:
			case MoveRY:
			case MoveRZ:
			case MoveSpeed:
			case MoveX:
			case MoveY:
			case MoveZ:
			{
				sampgdk::logprintf("*** Streamer_SetFloatData: Use MoveDynamicObject to adjust moving object data");
				return 0;
			}
			case RX:
			{
				o->second->rotation[0] = amx_ctof(params[4]);
				update = true;
				break;
			}
			case RY:
			{
				o->second->rotation[1] = amx_ctof(params[4]);
				update = true;
				break;
			}
			case RZ:
			{
				o->second->rotation[2] = amx_ctof(params[4]);
				update = true;
				break;
			}
			case StreamDistance:
			{
				o->second->streamDistance = amx_ctof(params[4]) * amx_ctof(params[4]);
				reassign = true;
				break;
			}
			case X:
			{
				if (o->second->move)
				{
					sampgdk::logprintf("*** Streamer_SetFloatData: Object must be stopped first");
					return 0;
				}
				o->second->position[0] = amx_ctof(params[4]);
				if (o->second->cell)
				{
					reassign = true;
				}
				update = true;
				break;
			}
			case Y:
			{
				if (o->second->move)
				{
					sampgdk::logprintf("*** Streamer_SetFloatData: Object must be stopped first");
					return 0;
				}
				o->second->position[1] = amx_ctof(params[4]);
				if (o->second->cell)
				{
					reassign = true;
				}
				update = true;
				break;
			}
			case Z:
			{
				if (o->second->move)
				{
					sampgdk::logprintf("*** Streamer_SetFloatData: Object must be stopped first");
					return 0;
				}
				o->second->position[2] = amx_ctof(params[4]);
				update = true;
				break;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
			if (reassign)
			{
				core->getGrid()->removeObject(o->second, true);
			}
			if (update)
			{
				for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
				{
					boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
					if (i != p->second.internalObjects.end())
					{
						DestroyPlayerObject(p->first, i->second);
						i->second = CreatePlayerObject(p->first, o->second->modelID, o->second->position[0], o->second->position[1], o->second->position[2], o->second->rotation[0], o->second->rotation[1], o->second->rotation[2], o->second->drawDistance);
						if (o->second->attach)
						{
							AttachPlayerObjectToVehicle(p->first, i->second, o->second->attach->vehicle, o->second->attach->offset[0], o->second->attach->offset[1], o->second->attach->offset[2], o->second->attach->rotation[0], o->second->attach->rotation[1], o->second->attach->rotation[2]);
						}
						else if (o->second->move)
						{
							MovePlayerObject(p->first, i->second, o->second->move->position.get<0>()[0], o->second->move->position.get<0>()[1], o->second->move->position.get<0>()[2], o->second->move->speed, o->second->move->rotation.get<0>()[0], o->second->move->rotation.get<0>()[1], o->second->move->rotation.get<0>()[2]);
						}
						for (boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.begin(); m != o->second->materials.end(); ++m)
						{
							if (m->second.main)
							{
								SetPlayerObjectMaterial(p->first, i->second, m->first, m->second.main->modelID, m->second.main->txdFileName.c_str(), m->second.main->textureName.c_str(), m->second.main->materialColor);
							}
							else if (m->second.text)
							{
								SetPlayerObjectMaterialText(p->first, i->second, m->second.text->materialText.c_str(), m->first, m->second.text->materialSize, m->second.text->fontFace.c_str(), m->second.text->fontSize, m->second.text->bold, m->second.text->fontColor, m->second.text->backColor, m->second.text->textAlignment);
							}
						}
					}
				}
			}
			if (reassign || update)
			{
				return 1;
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(static_cast<int>(params[2]));
		if (p != core->getData()->pickups.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case StreamDistance:
			{
				p->second->streamDistance = amx_ctof(params[4]) * amx_ctof(params[4]);
				reassign = true;
				break;
			}
			case X:
			{
				p->second->position[0] = amx_ctof(params[4]);
				if (p->second->cell)
				{
					reassign = true;
				}
				update = true;
				break;
			}
			case Y:
			{
				p->second->position[1] = amx_ctof(params[4]);
				if (p->second->cell)
				{
					reassign = true;
				}
				update = true;
				break;
			}
			case Z:
			{
				p->second->position[2] = amx_ctof(params[4]);
				update = true;
				break;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
			if (reassign)
			{
				core->getGrid()->removePickup(p->second, true);
			}
			if (update)
			{
				boost::unordered_map<int, int>::iterator i = core->getStreamer()->internalPickups.find(p->first);
				if (i != core->getStreamer()->internalPickups.end())
				{
					DestroyPickup(i->second);
					i->second = CreatePickup(p->second->modelID, p->second->type, p->second->position[0], p->second->position[1], p->second->position[2], p->second->worldID);
				}
			}
			if (reassign || update)
			{
				return 1;
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(static_cast<int>(params[2]));
		if (c != core->getData()->checkpoints.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case Size:
			{
				c->second->size = amx_ctof(params[4]);
				update = true;
				break;
			}
			case StreamDistance:
			{
				c->second->streamDistance = amx_ctof(params[4]) * amx_ctof(params[4]);
				reassign = true;
				break;
			}
			case X:
			{
				c->second->position[0] = amx_ctof(params[4]);
				if (c->second->cell)
				{
					reassign = true;
				}
				update = true;
				break;
			}
			case Y:
			{
				c->second->position[1] = amx_ctof(params[4]);
				if (c->second->cell)
				{
					reassign = true;
				}
				update = true;
				break;
			}
			case Z:
			{
				c->second->position[2] = amx_ctof(params[4]);
				update = true;
				break;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
			if (reassign)
			{
				core->getGrid()->removeCheckpoint(c->second, true);
			}
			if (update)
			{
				for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
				{
					if (p->second.visibleCheckpoint == c->first)
					{
						DisablePlayerCheckpoint(p->first);
						SetPlayerCheckpoint(p->first, c->second->position[0], c->second->position[1], c->second->position[2], c->second->size);
					}
				}
			}
			if (reassign || update)
			{
				return 1;
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(static_cast<int>(params[2]));
		if (r != core->getData()->raceCheckpoints.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case NextX:
			{
				r->second->next[0] = amx_ctof(params[4]);
				update = true;
				break;
			}
			case NextY:
			{
				r->second->next[1] = amx_ctof(params[4]);
				update = true;
				break;
			}
			case NextZ:
			{
				r->second->next[2] = amx_ctof(params[4]);
				update = true;
				break;
			}
			case Size:
			{
				r->second->size = amx_ctof(params[4]);
				update = true;
				break;
			}
			case StreamDistance:
			{
				r->second->streamDistance = amx_ctof(params[4]) * amx_ctof(params[4]);
				reassign = true;
				break;
			}
			case X:
			{
				r->second->position[0] = amx_ctof(params[4]);
				if (r->second->cell)
				{
					reassign = true;
				}
				update = true;
				break;
			}
			case Y:
			{
				r->second->position[1] = amx_ctof(params[4]);
				if (r->second->cell)
				{
					reassign = true;
				}
				update = true;
				break;
			}
			case Z:
			{
				r->second->position[2] = amx_ctof(params[4]);
				update = true;
				break;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
			if (reassign)
			{
				core->getGrid()->removeRaceCheckpoint(r->second, true);
			}
			if (update)
			{
				for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
				{
					if (p->second.visibleRaceCheckpoint == r->first)
					{
						DisablePlayerRaceCheckpoint(p->first);
						SetPlayerRaceCheckpoint(p->first, r->second->type, r->second->position[0], r->second->position[1], r->second->position[2], r->second->next[0], r->second->next[1], r->second->next[2], r->second->size);
					}
				}
			}
			if (reassign || update)
			{
				return 1;
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(static_cast<int>(params[2]));
		if (m != core->getData()->mapIcons.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case StreamDistance:
			{
				m->second->streamDistance = amx_ctof(params[4]) * amx_ctof(params[4]);
				reassign = true;
				break;
			}
			case X:
			{
				m->second->position[0] = amx_ctof(params[4]);
				if (m->second->cell)
				{
					reassign = true;
				}
				update = true;
				break;
			}
			case Y:
			{
				m->second->position[1] = amx_ctof(params[4]);
				if (m->second->cell)
				{
					reassign = true;
				}
				update = true;
				break;
			}
			case Z:
			{
				m->second->position[2] = amx_ctof(params[4]);
				update = true;
				break;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
			if (reassign)
			{
				core->getGrid()->removeMapIcon(m->second, true);
			}
			if (update)
			{
				for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
				{
					boost::unordered_map<int, int>::iterator i = p->second.internalMapIcons.find(m->first);
					if (i != p->second.internalMapIcons.end())
					{
						RemovePlayerMapIcon(p->first, i->second);
						SetPlayerMapIcon(p->first, i->second, m->second->position[0], m->second->position[1], m->second->position[2], m->second->type, m->second->color, m->second->style);
					}
				}
			}
			if (reassign || update)
			{
				return 1;
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(static_cast<int>(params[2]));
		if (t != core->getData()->textLabels.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case DrawDistance:
			{
				t->second->drawDistance = amx_ctof(params[4]);
				update = true;
				break;
			}
			case StreamDistance:
			{
				t->second->streamDistance = amx_ctof(params[4]) * amx_ctof(params[4]);
				reassign = true;
				break;
			}
			case AttachOffsetX:
			case X:
			{
				t->second->position[0] = amx_ctof(params[4]);
				if (t->second->cell)
				{
					reassign = true;
				}
				update = true;
				break;
			}
			case AttachOffsetY:
			case Y:
			{
				t->second->position[1] = amx_ctof(params[4]);
				if (t->second->cell)
				{
					reassign = true;
				}
				update = true;
				break;
			}
			case AttachOffsetZ:
			case Z:
			{
				t->second->position[2] = amx_ctof(params[4]);
				update = true;
				break;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
			if (reassign)
			{
				core->getGrid()->removeTextLabel(t->second, true);
			}
			if (update)
			{
				for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
				{
					boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(t->first);
					if (i != p->second.internalTextLabels.end())
					{
						DeletePlayer3DTextLabel(p->first, i->second);
						i->second = CreatePlayer3DTextLabel(p->first, t->second->text.c_str(), t->second->color, t->second->position[0], t->second->position[1], t->second->position[2], t->second->drawDistance, t->second->attach ? t->second->attach->player : INVALID_GENERIC_ID, t->second->attach ? t->second->attach->vehicle : INVALID_GENERIC_ID, t->second->testLOS);
					}
				}
			}
			if (reassign || update)
			{
				return 1;
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_AREA:
	{
		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[2]));
		if (a != core->getData()->areas.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case MaxX:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_RECTANGLE:
				{
					boost::get<Box2D>(a->second->position).max_corner()[0] = amx_ctof(params[4]);
					reassign = true;
					break;
				}
				case STREAMER_AREA_TYPE_CUBOID:
				{
					boost::get<Box3D>(a->second->position).max_corner()[0] = amx_ctof(params[4]);
					reassign = true;
					break;
				}
				}
				break;
			}
			case MaxY:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_RECTANGLE:
				{
					boost::get<Box2D>(a->second->position).max_corner()[1] = amx_ctof(params[4]);
					reassign = true;
					break;
				}
				case STREAMER_AREA_TYPE_CUBOID:
				{
					boost::get<Box3D>(a->second->position).max_corner()[1] = amx_ctof(params[4]);
					reassign = true;
					break;
				}
				}
				break;
			}
			case MaxZ:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_CUBOID:
				{
					boost::get<Box3D>(a->second->position).max_corner()[2] = amx_ctof(params[4]);
					return 1;
				}
				case STREAMER_AREA_TYPE_CYLINDER:
				case STREAMER_AREA_TYPE_POLYGON:
				{
					a->second->height[1] = amx_ctof(params[4]);
					return 0;
				}
				}
				return 0;
			}
			case MinX:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_RECTANGLE:
				{
					boost::get<Box2D>(a->second->position).min_corner()[0] = amx_ctof(params[4]);
					reassign = true;
					break;
				}
				case STREAMER_AREA_TYPE_CUBOID:
				{
					boost::get<Box3D>(a->second->position).min_corner()[0] = amx_ctof(params[4]);
					reassign = true;
					break;
				}
				}
				break;
			}
			case MinY:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_RECTANGLE:
				{
					boost::get<Box2D>(a->second->position).min_corner()[1] = amx_ctof(params[4]);
					reassign = true;
					break;
				}
				case STREAMER_AREA_TYPE_CUBOID:
				{
					boost::get<Box3D>(a->second->position).min_corner()[1] = amx_ctof(params[4]);
					reassign = true;
					break;
				}
				}
				break;
			}
			case MinZ:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_CUBOID:
				{
					boost::get<Box3D>(a->second->position).min_corner()[2] = amx_ctof(params[4]);
					return 1;
				}
				case STREAMER_AREA_TYPE_CYLINDER:
				case STREAMER_AREA_TYPE_POLYGON:
				{
					a->second->height[0] = amx_ctof(params[4]);
					return 1;
				}
				}
				return 0;
			}
			case Size:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_CIRCLE:
				case STREAMER_AREA_TYPE_CYLINDER:
				case STREAMER_AREA_TYPE_SPHERE:
				{
					a->second->size = amx_ctof(params[4]) * amx_ctof(params[4]);
					reassign = true;
					break;
				}
				}
				break;
			}
			case X:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_CIRCLE:
				case STREAMER_AREA_TYPE_CYLINDER:
				{
					boost::get<Eigen::Vector2f>(a->second->position)[0] = amx_ctof(params[4]);
					reassign = true;
					break;
				}
				case STREAMER_AREA_TYPE_SPHERE:
				{
					boost::get<Eigen::Vector3f>(a->second->position)[0] = amx_ctof(params[4]);
					reassign = true;
					break;
				}
				}
				break;
			}
			case Y:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_CIRCLE:
				case STREAMER_AREA_TYPE_CYLINDER:
				{
					boost::get<Eigen::Vector2f>(a->second->position)[1] = amx_ctof(params[4]);
					reassign = true;
					break;
				}
				case STREAMER_AREA_TYPE_SPHERE:
				{
					boost::get<Eigen::Vector3f>(a->second->position)[1] = amx_ctof(params[4]);
					reassign = true;
					break;
				}
				}
				break;
			}
			case Z:
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_SPHERE:
				{
					boost::get<Eigen::Vector3f>(a->second->position)[2] = amx_ctof(params[4]);
					return 1;
				}
				}
				return 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
			if (reassign)
			{
				switch (a->second->type)
				{
				case STREAMER_AREA_TYPE_RECTANGLE:
				{
					boost::geometry::correct(boost::get<Box2D>(a->second->position));
					a->second->size = static_cast<float>(boost::geometry::comparable_distance(boost::get<Box2D>(a->second->position).min_corner(), boost::get<Box2D>(a->second->position).max_corner()));
					break;
				}
				case STREAMER_AREA_TYPE_CUBOID:
				{
					boost::geometry::correct(boost::get<Box3D>(a->second->position));
					a->second->size = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(boost::get<Box3D>(a->second->position).min_corner()[0], boost::get<Box3D>(a->second->position).min_corner()[1]), Eigen::Vector2f(boost::get<Box3D>(a->second->position).max_corner()[0], boost::get<Box3D>(a->second->position).max_corner()[1])));
					break;
				}
				}
				core->getGrid()->removeArea(a->second, true);
			}
			if (reassign)
			{
				return 1;
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	default:
	{
		error = InvalidType;
		break;
	}
	}
	switch (error)
	{
	case InvalidData:
	{
		sampgdk::logprintf("*** Streamer_SetFloatData: Invalid data specified");
		break;
	}
	case InvalidID:
	{
		sampgdk::logprintf("*** Streamer_SetFloatData: Invalid ID specified");
		break;
	}
	case InvalidType:
	{
		sampgdk::logprintf("*** Streamer_SetFloatData: Invalid type specified");
		break;
	}
	}
	return 0;
}

int Manipulation::getIntData(AMX *amx, cell *params)
{
	int error = -1;
	switch (static_cast<int>(params[1]))
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[2]));
		if (o != core->getData()->objects.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case AttachedVehicle:
			{
				if (o->second->attach)
				{
					return o->second->attach->vehicle;
				}
				return INVALID_GENERIC_ID;
			}
			case ExtraID:
			{
				return Utility::getFirstValueInContainer(o->second->extras);
			}
			case InteriorID:
			{
				return Utility::getFirstValueInContainer(o->second->interiors);
			}
			case ModelID:
			{
				return o->second->modelID;
			}
			case PlayerID:
			{
				return Utility::getFirstValueInContainer(o->second->players);
			}
			case WorldID:
			{
				return Utility::getFirstValueInContainer(o->second->worlds);
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(static_cast<int>(params[2]));
		if (p != core->getData()->pickups.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case ExtraID:
			{
				return Utility::getFirstValueInContainer(p->second->extras);
			}
			case InteriorID:
			{
				return Utility::getFirstValueInContainer(p->second->interiors);
			}
			case ModelID:
			{
				return p->second->modelID;
			}
			case PlayerID:
			{
				return Utility::getFirstValueInContainer(p->second->players);
			}
			case Type:
			{
				return p->second->type;
			}
			case WorldID:
			{
				return Utility::getFirstValueInContainer(p->second->worlds);
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(static_cast<int>(params[2]));
		if (c != core->getData()->checkpoints.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case ExtraID:
			{
				return Utility::getFirstValueInContainer(c->second->extras);
			}
			case InteriorID:
			{
				return Utility::getFirstValueInContainer(c->second->interiors);
			}
			case PlayerID:
			{
				return Utility::getFirstValueInContainer(c->second->players);
			}
			case WorldID:
			{
				return Utility::getFirstValueInContainer(c->second->worlds);
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(static_cast<int>(params[2]));
		if (r != core->getData()->raceCheckpoints.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case ExtraID:
			{
				return Utility::getFirstValueInContainer(r->second->extras);
			}
			case InteriorID:
			{
				return Utility::getFirstValueInContainer(r->second->interiors);
			}
			case PlayerID:
			{
				return Utility::getFirstValueInContainer(r->second->players);
			}
			case Type:
			{
				return r->second->type;
			}
			case WorldID:
			{
				return Utility::getFirstValueInContainer(r->second->worlds);
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(static_cast<int>(params[2]));
		if (m != core->getData()->mapIcons.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case Color:
			{
				return m->second->color;
			}
			case ExtraID:
			{
				return Utility::getFirstValueInContainer(m->second->extras);
			}
			case InteriorID:
			{
				return Utility::getFirstValueInContainer(m->second->interiors);
			}
			case PlayerID:
			{
				return Utility::getFirstValueInContainer(m->second->players);
			}
			case Style:
			{
				return m->second->style;
			}
			case Type:
			{
				return m->second->type;
			}
			case WorldID:
			{
				return Utility::getFirstValueInContainer(m->second->worlds);
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(static_cast<int>(params[2]));
		if (t != core->getData()->textLabels.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case AttachedPlayer:
			{
				if (t->second->attach)
				{
					return t->second->attach->player;
				}
				return INVALID_GENERIC_ID;
			}
			case AttachedVehicle:
			{
				if (t->second->attach)
				{
					return t->second->attach->vehicle;
				}
				return INVALID_GENERIC_ID;
			}
			case Color:
			{
				return t->second->color;
			}
			case ExtraID:
			{
				return Utility::getFirstValueInContainer(t->second->extras);
			}
			case InteriorID:
			{
				return Utility::getFirstValueInContainer(t->second->interiors);
			}
			case PlayerID:
			{
				return Utility::getFirstValueInContainer(t->second->players);
			}
			case TestLOS:
			{
				return t->second->testLOS;
			}
			case WorldID:
			{
				return Utility::getFirstValueInContainer(t->second->worlds);
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_AREA:
	{
		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[2]));
		if (a != core->getData()->areas.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case AttachedObject:
			{
				if (a->second->attach)
				{
					return a->second->attach->object.get<0>();
				}
				return INVALID_GENERIC_ID;
			}
			case AttachedPlayer:
			{
				if (a->second->attach)
				{
					return a->second->attach->player;
				}
				return INVALID_GENERIC_ID;
			}
			case AttachedVehicle:
			{
				if (a->second->attach)
				{
					return a->second->attach->vehicle;
				}
				return INVALID_GENERIC_ID;
			}
			case ExtraID:
			{
				return Utility::getFirstValueInContainer(a->second->extras);
			}
			case InteriorID:
			{
				return Utility::getFirstValueInContainer(a->second->interiors);
			}
			case PlayerID:
			{
				return Utility::getFirstValueInContainer(a->second->players);
			}
			case Type:
			{
				return a->second->type;
			}
			case WorldID:
			{
				return Utility::getFirstValueInContainer(a->second->worlds);
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	default:
	{
		error = InvalidType;
		break;
	}
	}
	switch (error)
	{
	case InvalidData:
	{
		sampgdk::logprintf("*** Streamer_GetIntData: Invalid data specified");
		break;
	}
	case InvalidID:
	{
		sampgdk::logprintf("*** Streamer_GetIntData: Invalid ID specified");
		break;
	}
	case InvalidType:
	{
		sampgdk::logprintf("*** Streamer_GetIntData: Invalid type specified");
		break;
	}
	}
	return 0;
}

int Manipulation::setIntData(AMX *amx, cell *params)
{
	int error = -1;
	bool update = false;
	switch (static_cast<int>(params[1]))
	{
	case STREAMER_TYPE_OBJECT:
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[2]));
		if (o != core->getData()->objects.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case AttachedVehicle:
			{
				if (static_cast<int>(params[4]) != INVALID_GENERIC_ID)
				{
					o->second->attach = boost::intrusive_ptr<Item::Object::Attach>(new Item::Object::Attach);
					o->second->attach->vehicle = static_cast<int>(params[4]);
					o->second->attach->offset.setZero();
					o->second->attach->rotation.setZero();
					core->getStreamer()->attachedObjects.insert(o->second);
					update = true;
				}
				else
				{
					if (o->second->attach)
					{
						if (o->second->attach->vehicle != INVALID_GENERIC_ID)
						{
							o->second->attach.reset();
							core->getStreamer()->attachedObjects.erase(o->second);
							core->getGrid()->removeObject(o->second, true);
							update = true;
						}
					}
				}
				break;
			}
			case ExtraID:
			{
				return Utility::setFirstValueInContainer(o->second->extras, static_cast<int>(params[4])) != 0;
			}
			case InteriorID:
			{
				return Utility::setFirstValueInContainer(o->second->interiors, static_cast<int>(params[4])) != 0;
			}
			case ModelID:
			{
				o->second->modelID = static_cast<int>(params[4]);
				update = true;
				break;
			}
			case PlayerID:
			{
				return Utility::setFirstValueInContainer(o->second->players, static_cast<int>(params[4])) != 0;
			}
			case WorldID:
			{
				return Utility::setFirstValueInContainer(o->second->worlds, static_cast<int>(params[4])) != 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
			if (update)
			{
				for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
				{
					boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
					if (i != p->second.internalObjects.end())
					{
						DestroyPlayerObject(p->first, i->second);
						i->second = CreatePlayerObject(p->first, o->second->modelID, o->second->position[0], o->second->position[1], o->second->position[2], o->second->rotation[0], o->second->rotation[1], o->second->rotation[2], o->second->drawDistance);
						if (o->second->attach)
						{
							AttachPlayerObjectToVehicle(p->first, i->second, o->second->attach->vehicle, o->second->attach->offset[0], o->second->attach->offset[1], o->second->attach->offset[2], o->second->attach->rotation[0], o->second->attach->rotation[1], o->second->attach->rotation[2]);
						}
						else if (o->second->move)
						{
							MovePlayerObject(p->first, i->second, o->second->move->position.get<0>()[0], o->second->move->position.get<0>()[1], o->second->move->position.get<0>()[2], o->second->move->speed, o->second->move->rotation.get<0>()[0], o->second->move->rotation.get<0>()[1], o->second->move->rotation.get<0>()[2]);
						}
						for (boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.begin(); m != o->second->materials.end(); ++m)
						{
							if (m->second.main)
							{
								SetPlayerObjectMaterial(p->first, i->second, m->first, m->second.main->modelID, m->second.main->txdFileName.c_str(), m->second.main->textureName.c_str(), m->second.main->materialColor);
							}
							else if (m->second.text)
							{
								SetPlayerObjectMaterialText(p->first, i->second, m->second.text->materialText.c_str(), m->first, m->second.text->materialSize, m->second.text->fontFace.c_str(), m->second.text->fontSize, m->second.text->bold, m->second.text->fontColor, m->second.text->backColor, m->second.text->textAlignment);
							}
						}
					}
				}
				if (update)
				{
					return 1;
				}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(static_cast<int>(params[2]));
		if (p != core->getData()->pickups.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case ExtraID:
			{
				return Utility::setFirstValueInContainer(p->second->extras, static_cast<int>(params[4])) != 0;
			}
			case InteriorID:
			{
				return Utility::setFirstValueInContainer(p->second->interiors, static_cast<int>(params[4])) != 0;
			}
			case ModelID:
			{
				p->second->modelID = static_cast<int>(params[4]);
				update = true;
				break;
			}
			case PlayerID:
			{
				return Utility::setFirstValueInContainer(p->second->players, static_cast<int>(params[4])) != 0;
			}
			case Type:
			{
				p->second->type = static_cast<int>(params[4]);
				update = true;
				break;
			}
			case WorldID:
			{
				return Utility::setFirstValueInContainer(p->second->worlds, static_cast<int>(params[4])) != 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
			if (update)
			{
				boost::unordered_map<int, int>::iterator i = core->getStreamer()->internalPickups.find(p->first);
				if (i != core->getStreamer()->internalPickups.end())
				{
					DestroyPickup(i->second);
					i->second = CreatePickup(p->second->modelID, p->second->type, p->second->position[0], p->second->position[1], p->second->position[2], p->second->worldID);
				}
			}
			if (update)
			{
				return 1;
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_CP:
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(static_cast<int>(params[2]));
		if (c != core->getData()->checkpoints.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case ExtraID:
			{
				return Utility::setFirstValueInContainer(c->second->extras, static_cast<int>(params[4])) != 0;
			}
			case InteriorID:
			{
				return Utility::setFirstValueInContainer(c->second->interiors, static_cast<int>(params[4])) != 0;
				update = true;
				break;
			}
			case PlayerID:
			{
				return Utility::setFirstValueInContainer(c->second->players, static_cast<int>(params[4])) != 0;
			}
			case WorldID:
			{
				return Utility::setFirstValueInContainer(c->second->worlds, static_cast<int>(params[4])) != 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(static_cast<int>(params[2]));
		if (r != core->getData()->raceCheckpoints.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case ExtraID:
			{
				return Utility::setFirstValueInContainer(r->second->extras, static_cast<int>(params[4])) != 0;
			}
			case InteriorID:
			{
				return Utility::setFirstValueInContainer(r->second->interiors, static_cast<int>(params[4])) != 0;
			}
			case PlayerID:
			{
				return Utility::setFirstValueInContainer(r->second->players, static_cast<int>(params[4])) != 0;
			}
			case Type:
			{
				r->second->type = static_cast<int>(params[4]);
				update = true;
				break;
			}
			case WorldID:
			{
				return Utility::setFirstValueInContainer(r->second->worlds, static_cast<int>(params[4])) != 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
			if (update)
			{
				for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
				{
					if (p->second.visibleRaceCheckpoint == r->first)
					{
						DisablePlayerRaceCheckpoint(p->first);
						SetPlayerRaceCheckpoint(p->first, r->second->type, r->second->position[0], r->second->position[1], r->second->position[2], r->second->next[0], r->second->next[1], r->second->next[2], r->second->size);
					}
				}
			}
			if (update)
			{
				return 1;
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(static_cast<int>(params[2]));
		if (m != core->getData()->mapIcons.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case Color:
			{
				m->second->color = static_cast<int>(params[4]);
				update = true;
				break;
			}
			case ExtraID:
			{
				return Utility::setFirstValueInContainer(m->second->extras, static_cast<int>(params[4])) != 0;
			}
			case InteriorID:
			{
				return Utility::setFirstValueInContainer(m->second->interiors, static_cast<int>(params[4])) != 0;
			}
			case PlayerID:
			{
				return Utility::setFirstValueInContainer(m->second->players, static_cast<int>(params[4])) != 0;
			}
			case Style:
			{
				m->second->style = static_cast<int>(params[4]);
				update = true;
				break;
			}
			case Type:
			{
				m->second->type = static_cast<int>(params[4]);
				update = true;
				break;
			}
			case WorldID:
			{
				return Utility::setFirstValueInContainer(m->second->worlds, static_cast<int>(params[4])) != 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
			if (update)
			{
				for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
				{
					boost::unordered_map<int, int>::iterator i = p->second.internalMapIcons.find(m->first);
					if (i != p->second.internalMapIcons.end())
					{
						RemovePlayerMapIcon(p->first, i->second);
						SetPlayerMapIcon(p->first, i->second, m->second->position[0], m->second->position[1], m->second->position[2], m->second->type, m->second->color, m->second->style);
					}
				}
			}
			if (update)
			{
				return 1;
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(static_cast<int>(params[2]));
		if (t != core->getData()->textLabels.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case AttachedPlayer:
			{
				if (static_cast<int>(params[4]) != INVALID_GENERIC_ID)
				{
					t->second->attach = boost::intrusive_ptr<Item::TextLabel::Attach>(new Item::TextLabel::Attach);
					t->second->attach->player = static_cast<int>(params[4]);
					t->second->attach->vehicle = INVALID_GENERIC_ID;
					core->getStreamer()->attachedTextLabels.insert(t->second);
					update = true;
				}
				else
				{
					if (t->second->attach)
					{
						if (t->second->attach->player != INVALID_GENERIC_ID)
						{
							t->second->attach.reset();
							core->getStreamer()->attachedTextLabels.erase(t->second);
							core->getGrid()->removeTextLabel(t->second, true);
							update = true;
						}
					}
				}
				break;
			}
			case AttachedVehicle:
			{
				if (static_cast<int>(params[4]) != INVALID_GENERIC_ID)
				{
					t->second->attach = boost::intrusive_ptr<Item::TextLabel::Attach>(new Item::TextLabel::Attach);
					t->second->attach->player = INVALID_GENERIC_ID;
					t->second->attach->vehicle = static_cast<int>(params[4]);
					core->getStreamer()->attachedTextLabels.insert(t->second);
					update = true;
				}
				else
				{
					if (t->second->attach)
					{
						if (t->second->attach->vehicle != INVALID_GENERIC_ID)
						{
							t->second->attach.reset();
							core->getStreamer()->attachedTextLabels.erase(t->second);
							core->getGrid()->removeTextLabel(t->second, true);
							update = true;
						}
					}
				}
				break;
			}
			case Color:
			{
				t->second->color = static_cast<int>(params[4]);
				update = true;
				break;
			}
			case ExtraID:
			{
				return Utility::setFirstValueInContainer(t->second->extras, static_cast<int>(params[4])) != 0;
			}
			case InteriorID:
			{
				return Utility::setFirstValueInContainer(t->second->interiors, static_cast<int>(params[4])) != 0;
			}
			case PlayerID:
			{
				return Utility::setFirstValueInContainer(t->second->players, static_cast<int>(params[4])) != 0;
			}
			case TestLOS:
			{
				t->second->testLOS = static_cast<int>(params[4]) != 0;
				update = true;
				break;
			}
			case WorldID:
			{
				return Utility::setFirstValueInContainer(t->second->worlds, static_cast<int>(params[4])) != 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
			if (update)
			{
				if (t->second->attach)
				{
					if (t->second->position.cwiseAbs().maxCoeff() > 50.0f)
					{
						t->second->position.setZero();
					}
				}
				for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
				{
					boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(t->first);
					if (i != p->second.internalTextLabels.end())
					{
						DeletePlayer3DTextLabel(p->first, i->second);
						i->second = CreatePlayer3DTextLabel(p->first, t->second->text.c_str(), t->second->color, t->second->position[0], t->second->position[1], t->second->position[2], t->second->drawDistance, t->second->attach ? t->second->attach->player : INVALID_GENERIC_ID, t->second->attach ? t->second->attach->vehicle : INVALID_GENERIC_ID, t->second->testLOS);
					}
				}
			}
			if (update)
			{
				return 1;
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	case STREAMER_TYPE_AREA:
	{
		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[2]));
		if (a != core->getData()->areas.end())
		{
			switch (static_cast<int>(params[3]))
			{
			case AttachedObject:
			{
				sampgdk::logprintf("*** Streamer_SetFloatData: Use AttachDynamicAreaToObject to adjust attached area data");
				return 0;
			}
			case AttachedPlayer:
			{
				if (static_cast<int>(params[4]) != INVALID_GENERIC_ID)
				{
					a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
					a->second->attach->object.get<0>() = INVALID_GENERIC_ID;
					a->second->attach->player = static_cast<int>(params[4]);
					a->second->attach->vehicle = INVALID_GENERIC_ID;
					core->getStreamer()->attachedAreas.insert(a->second);
					return 1;
				}
				else
				{
					if (a->second->attach)
					{
						if (a->second->attach->player != INVALID_GENERIC_ID)
						{
							a->second->attach.reset();
							core->getStreamer()->attachedAreas.erase(a->second);
							core->getGrid()->removeArea(a->second, true);
							return 1;
						}
					}
				}
				return 0;
			}
			case AttachedVehicle:
			{
				if (static_cast<int>(params[4]) != INVALID_GENERIC_ID)
				{
					a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
					a->second->attach->object.get<0>() = INVALID_GENERIC_ID;
					a->second->attach->player = INVALID_GENERIC_ID;
					a->second->attach->vehicle = static_cast<int>(params[4]);
					core->getStreamer()->attachedAreas.insert(a->second);
					return 1;
				}
				else
				{
					if (a->second->attach)
					{
						if (a->second->attach->vehicle != INVALID_GENERIC_ID)
						{
							a->second->attach.reset();
							core->getStreamer()->attachedAreas.erase(a->second);
							core->getGrid()->removeArea(a->second, true);
							return 1;
						}
					}
				}
				return 0;
			}
			case ExtraID:
			{
				return Utility::setFirstValueInContainer(a->second->extras, static_cast<int>(params[4])) != 0;
			}
			case InteriorID:
			{
				return Utility::setFirstValueInContainer(a->second->interiors, static_cast<int>(params[4])) != 0;
			}
			case PlayerID:
			{
				return Utility::setFirstValueInContainer(a->second->players, static_cast<int>(params[4])) != 0;
			}
			case WorldID:
			{
				return Utility::setFirstValueInContainer(a->second->worlds, static_cast<int>(params[4])) != 0;
			}
			default:
			{
				error = InvalidData;
				break;
			}
			}
		}
		else
		{
			error = InvalidID;
		}
		break;
	}
	default:
	{
		error = InvalidType;
		break;
	}
	}
	switch (error)
	{
	case InvalidData:
	{
		sampgdk::logprintf("*** Streamer_SetIntData: Invalid data specified");
		break;
	}
	case InvalidID:
	{
		sampgdk::logprintf("*** Streamer_SetIntData: Invalid ID specified");
		break;
	}
	case InvalidType:
	{
		sampgdk::logprintf("*** Streamer_SetIntData: Invalid type specified");
		break;
	}
	}
	return 0;
}

int Manipulation::getArrayData(AMX *amx, cell *params)
{
	int error = -1, result = 0;
	switch (static_cast<int>(params[1]))
	{
	case STREAMER_TYPE_OBJECT:
	{
		result = getArrayDataForItem(core->getData()->objects, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		result = getArrayDataForItem(core->getData()->pickups, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	case STREAMER_TYPE_CP:
	{
		result = getArrayDataForItem(core->getData()->checkpoints, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		result = getArrayDataForItem(core->getData()->raceCheckpoints, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		result = getArrayDataForItem(core->getData()->mapIcons, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		result = getArrayDataForItem(core->getData()->textLabels, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	case STREAMER_TYPE_AREA:
	{
		result = getArrayDataForItem(core->getData()->areas, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	default:
	{
		error = InvalidType;
		break;
	}
	}
	switch (error)
	{
	case InvalidData:
	{
		sampgdk::logprintf("*** Streamer_GetArrayData: Invalid data specified");
		break;
	}
	case InvalidID:
	{
		sampgdk::logprintf("*** Streamer_GetArrayData: Invalid ID specified");
		break;
	}
	case InvalidType:
	{
		sampgdk::logprintf("*** Streamer_GetArrayData: Invalid type specified");
		break;
	}
	default:
	{
		return result;
	}
	}
	return 0;
}

int Manipulation::setArrayData(AMX *amx, cell *params)
{
	int error = -1, result = 0;
	switch (static_cast<int>(params[1]))
	{
	case STREAMER_TYPE_OBJECT:
	{
		result = setArrayDataForItem(core->getData()->objects, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		result = setArrayDataForItem(core->getData()->pickups, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	case STREAMER_TYPE_CP:
	{
		result = setArrayDataForItem(core->getData()->checkpoints, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		result = setArrayDataForItem(core->getData()->raceCheckpoints, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		result = setArrayDataForItem(core->getData()->mapIcons, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		result = setArrayDataForItem(core->getData()->textLabels, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	case STREAMER_TYPE_AREA:
	{
		result = setArrayDataForItem(core->getData()->areas, amx, static_cast<int>(params[2]), static_cast<int>(params[3]), params[4], params[5], error);
		break;
	}
	default:
	{
		error = InvalidType;
		break;
	}
	}
	switch (error)
	{
	case InvalidData:
	{
		sampgdk::logprintf("*** Streamer_SetArrayData: Invalid data specified");
		break;
	}
	case InvalidID:
	{
		sampgdk::logprintf("*** Streamer_SetArrayData: Invalid ID specified");
		break;
	}
	case InvalidType:
	{
		sampgdk::logprintf("*** Streamer_SetArrayData: Invalid type specified");
		break;
	}
	default:
	{
		return result;
	}
	}
	return 0;
}

int Manipulation::isInArrayData(AMX *amx, cell *params)
{
	int error = -1, result = 0;
	switch (static_cast<int>(params[1]))
	{
	case STREAMER_TYPE_OBJECT:
	{
		result = isInArrayDataForItem(core->getData()->objects, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		result = isInArrayDataForItem(core->getData()->pickups, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_CP:
	{
		result = isInArrayDataForItem(core->getData()->checkpoints, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		result = isInArrayDataForItem(core->getData()->raceCheckpoints, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		result = isInArrayDataForItem(core->getData()->mapIcons, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		result = isInArrayDataForItem(core->getData()->textLabels, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_AREA:
	{
		result = isInArrayDataForItem(core->getData()->areas, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	default:
	{
		error = InvalidType;
		break;
	}
	}
	switch (error)
	{
	case InvalidData:
	{
		sampgdk::logprintf("*** Streamer_IsInArrayData: Invalid data specified");
		break;
	}
	case InvalidID:
	{
		sampgdk::logprintf("*** Streamer_IsInArrayData: Invalid ID specified");
		break;
	}
	case InvalidType:
	{
		sampgdk::logprintf("*** Streamer_IsInArrayData: Invalid type specified");
		break;
	}
	default:
	{
		return result;
	}
	}
	return 0;
}

int Manipulation::appendArrayData(AMX *amx, cell *params)
{
	int error = -1, result = 0;
	switch (static_cast<int>(params[1]))
	{
	case STREAMER_TYPE_OBJECT:
	{
		result = appendArrayDataForItem(core->getData()->objects, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		result = appendArrayDataForItem(core->getData()->pickups, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_CP:
	{
		result = appendArrayDataForItem(core->getData()->checkpoints, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		result = appendArrayDataForItem(core->getData()->raceCheckpoints, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		result = appendArrayDataForItem(core->getData()->mapIcons, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		result = appendArrayDataForItem(core->getData()->textLabels, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_AREA:
	{
		result = appendArrayDataForItem(core->getData()->areas, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	default:
	{
		error = InvalidType;
		break;
	}
	}
	switch (error)
	{
	case InvalidData:
	{
		sampgdk::logprintf("*** Streamer_AppendArrayData: Invalid data specified");
		break;
	}
	case InvalidID:
	{
		sampgdk::logprintf("*** Streamer_AppendArrayData: Invalid ID specified");
		break;
	}
	case InvalidType:
	{
		sampgdk::logprintf("*** Streamer_AppendArrayData: Invalid type specified");
		break;
	}
	default:
	{
		return result;
	}
	}
	return 0;
}

int Manipulation::removeArrayData(AMX *amx, cell *params)
{
	int error = -1, result = 0;
	switch (static_cast<int>(params[1]))
	{
	case STREAMER_TYPE_OBJECT:
	{
		result = removeArrayDataForItem(core->getData()->objects, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_PICKUP:
	{
		result = removeArrayDataForItem(core->getData()->pickups, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_CP:
	{
		result = removeArrayDataForItem(core->getData()->checkpoints, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		result = removeArrayDataForItem(core->getData()->raceCheckpoints, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		result = removeArrayDataForItem(core->getData()->mapIcons, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		result = removeArrayDataForItem(core->getData()->textLabels, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	case STREAMER_TYPE_AREA:
	{
		result = removeArrayDataForItem(core->getData()->areas, static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]), error);
		break;
	}
	default:
	{
		error = InvalidType;
		break;
	}
	}
	switch (error)
	{
	case InvalidData:
	{
		sampgdk::logprintf("*** Streamer_RemoveArrayData: Invalid data specified");
		break;
	}
	case InvalidID:
	{
		sampgdk::logprintf("*** Streamer_RemoveArrayData: Invalid ID specified");
		break;
	}
	case InvalidType:
	{
		sampgdk::logprintf("*** Streamer_RemoveArrayData: Invalid type specified");
		break;
	}
	default:
	{
		return result;
	}
	}
	return 0;
}


Player::Player(int playerID) : playerID(playerID)
{
	activeCheckpoint = 0;
	activeRaceCheckpoint = 0;
	enabledItems.set();
	interiorID = 0;
	position.setZero();
	updateWhenIdle = false;
	visibleCell = SharedCell(new Cell());
	visibleCheckpoint = 0;
	visibleRaceCheckpoint = 0;
	visibleObjects = core->getStreamer()->getVisibleItems(STREAMER_TYPE_OBJECT);
	visibleTextLabels = core->getStreamer()->getVisibleItems(STREAMER_TYPE_3D_TEXT_LABEL);
	worldID = 0;
}

Streamer::Streamer()
{
	averageUpdateTime = 0.0f;
	processingFinalPlayer = false;
	tickCount = 0;
	tickRate = 50;
	velocityBoundaries = boost::make_tuple(0.25f, 25.0f);
	visibleMapIcons = 100;
	visibleObjects = 500;
	visiblePickups = 4096;
	visibleTextLabels = 1024;
}

std::size_t Streamer::getVisibleItems(int type)
{
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		return visibleObjects;
	}
	case STREAMER_TYPE_PICKUP:
	{
		return visiblePickups;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		return visibleMapIcons;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		return visibleTextLabels;
	}
	}
	return 0;
}

bool Streamer::setVisibleItems(int type, std::size_t value)
{
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		visibleObjects = value;
		return true;
	}
	case STREAMER_TYPE_PICKUP:
	{
		visiblePickups = value;
		return true;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		visibleMapIcons = value;
		return true;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		visibleTextLabels = value;
		return true;
	}
	}
	return false;
}

void Streamer::calculateAverageUpdateTime()
{
	static boost::chrono::steady_clock::time_point currentTime, lastRecordedTime;
	static Eigen::Array<float, 5, 1> recordedTimes = Eigen::Array<float, 5, 1>::Zero();
	currentTime = boost::chrono::steady_clock::now();
	if (lastRecordedTime.time_since_epoch().count())
	{
		if (!(recordedTimes > 0).all())
		{
			boost::chrono::duration<float> elapsedTime = currentTime - lastRecordedTime;
			recordedTimes[(recordedTimes > 0).count()] = elapsedTime.count();
		}
		else
		{
			averageUpdateTime = recordedTimes.mean() * 50.0f;
			recordedTimes.setZero();
		}
	}
	lastRecordedTime = currentTime;
}

void Streamer::startAutomaticUpdate()
{
	if (tickCount >= tickRate)
	{
		calculateAverageUpdateTime();
		processActiveItems();
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			processingFinalPlayer = isLastPlayer(p, core->getData()->players);
			performPlayerUpdate(p->second, true);
		}
		processingFinalPlayer = false;
		tickCount = 0;
	}
	++tickCount;
}

void Streamer::startManualUpdate(Player &player, bool getData)
{
	if (getData)
	{
		player.interiorID = GetPlayerInterior(player.playerID);
		player.worldID = GetPlayerVirtualWorld(player.playerID);
		GetPlayerPos(player.playerID, &player.position[0], &player.position[1], &player.position[2]);
	}
	processActiveItems();
	performPlayerUpdate(player, false);
}

void Streamer::performPlayerUpdate(Player &player, bool automatic)
{
	Eigen::Vector3f delta = Eigen::Vector3f::Zero(), position = player.position;
	int state = GetPlayerState(player.playerID);
	bool update = true;
	if (automatic)
	{
		player.interiorID = GetPlayerInterior(player.playerID);
		player.worldID = GetPlayerVirtualWorld(player.playerID);
		GetPlayerPos(player.playerID, &player.position[0], &player.position[1], &player.position[2]);
		if (state != PLAYER_STATE_NONE && state != PLAYER_STATE_WASTED)
		{
			if (player.position != position)
			{
				position = player.position;
				Eigen::Vector3f velocity = Eigen::Vector3f::Zero();
				if (state == PLAYER_STATE_ONFOOT)
				{
					GetPlayerVelocity(player.playerID, &velocity[0], &velocity[1], &velocity[2]);
				}
				else if (state == PLAYER_STATE_DRIVER || state == PLAYER_STATE_PASSENGER)
				{
					GetVehicleVelocity(GetPlayerVehicleID(player.playerID), &velocity[0], &velocity[1], &velocity[2]);
				}
				float velocityNorm = velocity.squaredNorm();
				if (velocityNorm >= velocityBoundaries.get<0>() && velocityNorm <= velocityBoundaries.get<1>())
				{
					delta = velocity * averageUpdateTime;
					player.position += delta;
				}
			}
			else
			{
				update = player.updateWhenIdle;
			}
		}
		else
		{
			update = false;
		}
	}
	std::vector<SharedCell> cells;
	if (update)
	{
		core->getGrid()->findAllCells(player, cells);
		if (!cells.empty())
		{
			if (!core->getData()->objects.empty() && player.enabledItems[STREAMER_TYPE_OBJECT] && !IsPlayerNPC(player.playerID))
			{
				processObjects(player, cells);
			}
			if (!core->getData()->checkpoints.empty() && player.enabledItems[STREAMER_TYPE_CP])
			{
				processCheckpoints(player, cells);
			}
			if (!core->getData()->raceCheckpoints.empty() && player.enabledItems[STREAMER_TYPE_RACE_CP])
			{
				processRaceCheckpoints(player, cells);
			}
			if (!core->getData()->mapIcons.empty() && player.enabledItems[STREAMER_TYPE_MAP_ICON] && !IsPlayerNPC(player.playerID))
			{
				processMapIcons(player, cells);
			}
			if (!core->getData()->textLabels.empty() && player.enabledItems[STREAMER_TYPE_3D_TEXT_LABEL] && !IsPlayerNPC(player.playerID))
			{
				processTextLabels(player, cells);
			}
			if (!core->getData()->areas.empty() && player.enabledItems[STREAMER_TYPE_AREA])
			{
				if (!delta.isZero())
				{
					player.position = position;
				}
				processAreas(player, cells);
				if (!delta.isZero())
				{
					player.position += delta;
				}
			}
		}
	}
	if (automatic)
	{
		if (!core->getData()->pickups.empty())
		{
			if (!update)
			{
				core->getGrid()->findMinimalCells(player, cells);
			}
			processPickups(player, cells);
		}
		if (!delta.isZero())
		{
			player.position = position;
		}
		executeCallbacks();
	}
}

void Streamer::executeCallbacks()
{
	for (std::vector<boost::tuple<int, int> >::const_iterator c = areaLeaveCallbacks.begin(); c != areaLeaveCallbacks.end(); ++c)
	{
		StreamerLibrary::OnPlayerLeaveDynamicArea(c->get<1>(), c->get<0>());
	}
	areaLeaveCallbacks.clear();
	for (std::vector<boost::tuple<int, int> >::const_iterator c = areaEnterCallbacks.begin(); c != areaEnterCallbacks.end(); ++c)
	{
		StreamerLibrary::OnPlayerEnterDynamicArea(c->get<1>(), c->get<0>());
	}
	areaEnterCallbacks.clear();
	for (std::vector<int>::const_iterator c = objectMoveCallbacks.begin(); c != objectMoveCallbacks.end(); ++c)
	{
		StreamerLibrary::OnDynamicObjectMoved(*c);
	}
	objectMoveCallbacks.clear();
}

void Streamer::processAreas(Player &player, const std::vector<SharedCell> &cells)
{
	for (std::vector<SharedCell>::const_iterator c = cells.begin(); c != cells.end(); ++c)
	{
		for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = (*c)->areas.begin(); a != (*c)->areas.end(); ++a)
		{
			bool in = false;
			if (checkPlayer(a->second->players, player.playerID, a->second->interiors, player.interiorID, a->second->worlds, player.worldID))
			{
				boost::unordered_set<int>::iterator d = player.disabledAreas.find(a->first);
				if (d == player.disabledAreas.end())
				{
					in = Utility::isPointInArea(player.position, a->second);
				}
			}
			boost::unordered_set<int>::iterator i = player.internalAreas.find(a->first);
			if (in)
			{
				if (i == player.internalAreas.end())
				{
					player.internalAreas.insert(a->first);
					areaEnterCallbacks.push_back(boost::make_tuple(a->first, player.playerID));
				}
				if (a->second->cell)
				{
					player.visibleCell->areas.insert(*a);
				}
			}
			else
			{
				if (i != player.internalAreas.end())
				{
					player.internalAreas.quick_erase(i);
					areaLeaveCallbacks.push_back(boost::make_tuple(a->first, player.playerID));
				}
			}
		}
	}
}

void Streamer::processCheckpoints(Player &player, const std::vector<SharedCell> &cells)
{
	std::multimap<float, Item::SharedCheckpoint> discoveredCheckpoints;
	for (std::vector<SharedCell>::const_iterator c = cells.begin(); c != cells.end(); ++c)
	{
		for (boost::unordered_map<int, Item::SharedCheckpoint>::const_iterator d = (*c)->checkpoints.begin(); d != (*c)->checkpoints.end(); ++d)
		{
			float distance = std::numeric_limits<float>::infinity();
			if (checkPlayer(d->second->players, player.playerID, d->second->interiors, player.interiorID, d->second->worlds, player.worldID))
			{
				boost::unordered_set<int>::iterator e = player.disabledCheckpoints.find(d->first);
				if (e == player.disabledCheckpoints.end())
				{
					distance = static_cast<float>(boost::geometry::comparable_distance(player.position, d->second->position));
				}
			}
			if (distance <= d->second->streamDistance)
			{
				discoveredCheckpoints.insert(std::make_pair(distance, d->second));
			}
			else
			{
				if (d->first == player.visibleCheckpoint)
				{
					DisablePlayerCheckpoint(player.playerID);
					player.activeCheckpoint = 0;
					player.visibleCheckpoint = 0;
				}
			}
		}
	}
	if (!discoveredCheckpoints.empty())
	{
		std::multimap<float, Item::SharedCheckpoint>::iterator d = discoveredCheckpoints.begin();
		if (d->second->checkpointID != player.visibleCheckpoint)
		{
			if (player.visibleCheckpoint)
			{
				DisablePlayerCheckpoint(player.playerID);
				player.activeCheckpoint = 0;
			}
			SetPlayerCheckpoint(player.playerID, d->second->position[0], d->second->position[1], d->second->position[2], d->second->size);
			player.visibleCheckpoint = d->second->checkpointID;
		}
		if (d->second->cell)
		{
			player.visibleCell->checkpoints.insert(std::make_pair(d->second->checkpointID, d->second));
		}
	}
}

void Streamer::processMapIcons(Player &player, const std::vector<SharedCell> &cells)
{
	std::multimap<float, Item::SharedMapIcon> discoveredMapIcons, existingMapIcons;
	for (std::vector<SharedCell>::const_iterator c = cells.begin(); c != cells.end(); ++c)
	{
		for (boost::unordered_map<int, Item::SharedMapIcon>::const_iterator m = (*c)->mapIcons.begin(); m != (*c)->mapIcons.end(); ++m)
		{
			float distance = std::numeric_limits<float>::infinity();
			if (checkPlayer(m->second->players, player.playerID, m->second->interiors, player.interiorID, m->second->worlds, player.worldID))
			{
				distance = static_cast<float>(boost::geometry::comparable_distance(player.position, m->second->position));
			}
			boost::unordered_map<int, int>::iterator i = player.internalMapIcons.find(m->first);
			if (distance <= m->second->streamDistance)
			{
				if (i == player.internalMapIcons.end())
				{
					discoveredMapIcons.insert(std::make_pair(distance, m->second));
				}
				else
				{
					if (m->second->cell)
					{
						player.visibleCell->mapIcons.insert(*m);
					}
					existingMapIcons.insert(std::make_pair(distance, m->second));
				}
			}
			else
			{
				if (i != player.internalMapIcons.end())
				{
					RemovePlayerMapIcon(player.playerID, i->second);
					player.mapIconIdentifier.remove(i->second, player.internalMapIcons.size());
					player.internalMapIcons.quick_erase(i);
				}
			}
		}
	}
	for (std::multimap<float, Item::SharedMapIcon>::iterator d = discoveredMapIcons.begin(); d != discoveredMapIcons.end(); ++d)
	{
		if (player.internalMapIcons.size() == visibleMapIcons)
		{
			std::multimap<float, Item::SharedMapIcon>::reverse_iterator e = existingMapIcons.rbegin();
			if (e != existingMapIcons.rend())
			{
				if (d->first < e->first)
				{
					boost::unordered_map<int, int>::iterator i = player.internalMapIcons.find(e->second->mapIconID);
					if (i != player.internalMapIcons.end())
					{
						RemovePlayerMapIcon(player.playerID, i->second);
						player.mapIconIdentifier.remove(i->second, player.internalMapIcons.size());
						player.internalMapIcons.quick_erase(i);
					}
					if (e->second->cell)
					{
						player.visibleCell->mapIcons.erase(e->second->mapIconID);
					}
					existingMapIcons.erase(--e.base());
				}
			}
			if (player.internalMapIcons.size() == visibleMapIcons)
			{
				break;
			}
		}
		int internalID = player.mapIconIdentifier.get();
		SetPlayerMapIcon(player.playerID, internalID, d->second->position[0], d->second->position[1], d->second->position[2], d->second->type, d->second->color, d->second->style);
		player.internalMapIcons.insert(std::make_pair(d->second->mapIconID, internalID));
		if (d->second->cell)
		{
			player.visibleCell->mapIcons.insert(std::make_pair(d->second->mapIconID, d->second));
		}
	}
}

void Streamer::processObjects(Player &player, const std::vector<SharedCell> &cells)
{
	std::multimap<float, Item::SharedObject> discoveredObjects, existingObjects;
	for (std::vector<SharedCell>::const_iterator c = cells.begin(); c != cells.end(); ++c)
	{
		for (boost::unordered_map<int, Item::SharedObject>::const_iterator o = (*c)->objects.begin(); o != (*c)->objects.end(); ++o)
		{
			float distance = std::numeric_limits<float>::infinity();
			if (checkPlayer(o->second->players, player.playerID, o->second->interiors, player.interiorID, o->second->worlds, player.worldID))
			{
				if (o->second->attach)
				{
					distance = static_cast<float>(boost::geometry::comparable_distance(player.position, o->second->attach->position));
				}
				else
				{
					distance = static_cast<float>(boost::geometry::comparable_distance(player.position, o->second->position));
				}
			}
			boost::unordered_map<int, int>::iterator i = player.internalObjects.find(o->first);
			if (distance <= o->second->streamDistance)
			{
				if (i == player.internalObjects.end())
				{
					discoveredObjects.insert(std::make_pair(distance, o->second));
				}
				else
				{
					if (o->second->cell)
					{
						player.visibleCell->objects.insert(*o);
					}
					existingObjects.insert(std::make_pair(distance, o->second));
				}
			}
			else
			{
				if (i != player.internalObjects.end())
				{
					DestroyPlayerObject(player.playerID, i->second);
					player.internalObjects.quick_erase(i);
				}
			}
		}
	}
	for (std::multimap<float, Item::SharedObject>::iterator d = discoveredObjects.begin(); d != discoveredObjects.end(); ++d)
	{
		if (player.internalObjects.size() == player.visibleObjects)
		{
			std::multimap<float, Item::SharedObject>::reverse_iterator e = existingObjects.rbegin();
			if (e != existingObjects.rend())
			{
				if (d->first < e->first)
				{
					boost::unordered_map<int, int>::iterator i = player.internalObjects.find(e->second->objectID);
					if (i != player.internalObjects.end())
					{
						DestroyPlayerObject(player.playerID, i->second);
						player.internalObjects.quick_erase(i);
					}
					if (e->second->cell)
					{
						player.visibleCell->objects.erase(e->second->objectID);
					}
					existingObjects.erase(--e.base());
				}
			}
		}
		if (player.internalObjects.size() == visibleObjects)
		{
			player.visibleObjects = player.internalObjects.size();
			break;
		}
		int internalID = CreatePlayerObject(player.playerID, d->second->modelID, d->second->position[0], d->second->position[1], d->second->position[2], d->second->rotation[0], d->second->rotation[1], d->second->rotation[2], d->second->drawDistance);
		if (internalID == INVALID_GENERIC_ID)
		{
			player.visibleObjects = player.internalObjects.size();
			break;
		}
		if (d->second->attach)
		{
			AttachPlayerObjectToVehicle(player.playerID, internalID, d->second->attach->vehicle, d->second->attach->offset[0], d->second->attach->offset[1], d->second->attach->offset[2], d->second->attach->rotation[0], d->second->attach->rotation[1], d->second->attach->rotation[2]);
		}
		else if (d->second->move)
		{
			MovePlayerObject(player.playerID, internalID, d->second->move->position.get<0>()[0], d->second->move->position.get<0>()[1], d->second->move->position.get<0>()[2], d->second->move->speed, d->second->move->rotation.get<0>()[0], d->second->move->rotation.get<0>()[1], d->second->move->rotation.get<0>()[2]);
		}
		for (boost::unordered_map<int, Item::Object::Material>::iterator m = d->second->materials.begin(); m != d->second->materials.end(); ++m)
		{
			if (m->second.main)
			{
				SetPlayerObjectMaterial(player.playerID, internalID, m->first, m->second.main->modelID, m->second.main->txdFileName.c_str(), m->second.main->textureName.c_str(), m->second.main->materialColor);
			}
			else if (m->second.text)
			{
				SetPlayerObjectMaterialText(player.playerID, internalID, m->second.text->materialText.c_str(), m->first, m->second.text->materialSize, m->second.text->fontFace.c_str(), m->second.text->fontSize, m->second.text->bold, m->second.text->fontColor, m->second.text->backColor, m->second.text->textAlignment);
			}
		}
		player.internalObjects.insert(std::make_pair(d->second->objectID, internalID));
		if (d->second->cell)
		{
			player.visibleCell->objects.insert(std::make_pair(d->second->objectID, d->second));
		}
	}
}

void Streamer::processPickups(Player &player, const std::vector<SharedCell> &cells)
{
	static boost::unordered_map<int, Item::SharedPickup> discoveredPickups;
	for (std::vector<SharedCell>::const_iterator c = cells.begin(); c != cells.end(); ++c)
	{
		for (boost::unordered_map<int, Item::SharedPickup>::const_iterator p = (*c)->pickups.begin(); p != (*c)->pickups.end(); ++p)
		{
			boost::unordered_map<int, Item::SharedPickup>::iterator d = discoveredPickups.find(p->first);
			if (d == discoveredPickups.end())
			{
				if (checkPlayer(p->second->players, player.playerID, p->second->interiors, player.interiorID, p->second->worlds, player.worldID))
				{
					if (boost::geometry::comparable_distance(player.position, p->second->position) <= p->second->streamDistance)
					{
						boost::unordered_map<int, int>::iterator i = internalPickups.find(p->first);
						if (i == internalPickups.end())
						{
							p->second->worldID = !p->second->worlds.empty() ? player.worldID : -1;
						}
						discoveredPickups.insert(*p);
					}
				}
			}
		}
	}
	if (processingFinalPlayer)
	{
		boost::unordered_map<int, int>::iterator i = internalPickups.begin();
		while (i != internalPickups.end())
		{
			boost::unordered_map<int, Item::SharedPickup>::iterator d = discoveredPickups.find(i->first);
			if (d == discoveredPickups.end())
			{
				DestroyPickup(i->second);
				i = internalPickups.erase(i);
			}
			else
			{
				discoveredPickups.erase(d);
				++i;
			}
		}
		for (boost::unordered_map<int, Item::SharedPickup>::iterator d = discoveredPickups.begin(); d != discoveredPickups.end(); ++d)
		{
			if (internalPickups.size() == visiblePickups)
			{
				break;
			}
			int internalID = CreatePickup(d->second->modelID, d->second->type, d->second->position[0], d->second->position[1], d->second->position[2], d->second->worldID);
			if (internalID == INVALID_ALTERNATE_ID)
			{
				break;
			}
			internalPickups.insert(std::make_pair(d->second->pickupID, internalID));
		}
		discoveredPickups.clear();
	}
}

void Streamer::processRaceCheckpoints(Player &player, const std::vector<SharedCell> &cells)
{
	std::multimap<float, Item::SharedRaceCheckpoint> discoveredRaceCheckpoints;
	for (std::vector<SharedCell>::const_iterator c = cells.begin(); c != cells.end(); ++c)
	{
		for (boost::unordered_map<int, Item::SharedRaceCheckpoint>::const_iterator r = (*c)->raceCheckpoints.begin(); r != (*c)->raceCheckpoints.end(); ++r)
		{
			float distance = std::numeric_limits<float>::infinity();
			if (checkPlayer(r->second->players, player.playerID, r->second->interiors, player.interiorID, r->second->worlds, player.worldID))
			{
				boost::unordered_set<int>::iterator d = player.disabledRaceCheckpoints.find(r->first);
				if (d == player.disabledRaceCheckpoints.end())
				{
					distance = static_cast<float>(boost::geometry::comparable_distance(player.position, r->second->position));
				}
			}
			if (distance <= r->second->streamDistance)
			{
				discoveredRaceCheckpoints.insert(std::make_pair(distance, r->second));
			}
			else
			{
				if (r->first == player.visibleRaceCheckpoint)
				{
					DisablePlayerRaceCheckpoint(player.playerID);
					player.activeRaceCheckpoint = 0;
					player.visibleRaceCheckpoint = 0;
				}
			}
		}
	}
	if (!discoveredRaceCheckpoints.empty())
	{
		std::multimap<float, Item::SharedRaceCheckpoint>::iterator d = discoveredRaceCheckpoints.begin();
		if (d->second->raceCheckpointID != player.visibleRaceCheckpoint)
		{
			if (player.visibleRaceCheckpoint)
			{
				DisablePlayerRaceCheckpoint(player.playerID);
				player.activeRaceCheckpoint = 0;
			}
			SetPlayerRaceCheckpoint(player.playerID, d->second->type, d->second->position[0], d->second->position[1], d->second->position[2], d->second->next[0], d->second->next[1], d->second->next[2], d->second->size);
			player.visibleRaceCheckpoint = d->second->raceCheckpointID;
		}
		if (d->second->cell)
		{
			player.visibleCell->raceCheckpoints.insert(std::make_pair(d->second->raceCheckpointID, d->second));
		}
	}
}

void Streamer::processTextLabels(Player &player, const std::vector<SharedCell> &cells)
{
	std::multimap<float, Item::SharedTextLabel> discoveredTextLabels, existingTextLabels;
	for (std::vector<SharedCell>::const_iterator c = cells.begin(); c != cells.end(); ++c)
	{
		for (boost::unordered_map<int, Item::SharedTextLabel>::const_iterator t = (*c)->textLabels.begin(); t != (*c)->textLabels.end(); ++t)
		{
			float distance = std::numeric_limits<float>::infinity();
			if (checkPlayer(t->second->players, player.playerID, t->second->interiors, player.interiorID, t->second->worlds, player.worldID))
			{
				if (t->second->attach)
				{
					distance = static_cast<float>(boost::geometry::comparable_distance(player.position, t->second->attach->position));
				}
				else
				{
					distance = static_cast<float>(boost::geometry::comparable_distance(player.position, t->second->position));
				}
			}
			boost::unordered_map<int, int>::iterator i = player.internalTextLabels.find(t->first);
			if (distance <= t->second->streamDistance)
			{
				if (i == player.internalTextLabels.end())
				{
					discoveredTextLabels.insert(std::make_pair(distance, t->second));
				}
				else
				{
					if (t->second->cell)
					{
						player.visibleCell->textLabels.insert(*t);
					}
					existingTextLabels.insert(std::make_pair(distance, t->second));
				}
			}
			else
			{
				if (i != player.internalTextLabels.end())
				{
					DeletePlayer3DTextLabel(player.playerID, i->second);
					player.internalTextLabels.quick_erase(i);
				}
			}
		}
	}
	for (std::multimap<float, Item::SharedTextLabel>::iterator d = discoveredTextLabels.begin(); d != discoveredTextLabels.end(); ++d)
	{
		if (player.internalTextLabels.size() == player.visibleTextLabels)
		{
			std::multimap<float, Item::SharedTextLabel>::reverse_iterator e = existingTextLabels.rbegin();
			if (e != existingTextLabels.rend())
			{
				if (d->first < e->first)
				{
					boost::unordered_map<int, int>::iterator i = player.internalTextLabels.find(e->second->textLabelID);
					if (i != player.internalTextLabels.end())
					{
						DeletePlayer3DTextLabel(player.playerID, i->second);
						player.internalTextLabels.quick_erase(i);
					}
					if (e->second->cell)
					{
						player.visibleCell->textLabels.erase(e->second->textLabelID);
					}
					existingTextLabels.erase(--e.base());
				}
			}
		}
		if (player.internalTextLabels.size() == visibleTextLabels)
		{
			player.visibleTextLabels = player.internalTextLabels.size();
			break;
		}
		int internalID = CreatePlayer3DTextLabel(player.playerID, d->second->text.c_str(), d->second->color, d->second->position[0], d->second->position[1], d->second->position[2], d->second->drawDistance, d->second->attach ? d->second->attach->player : INVALID_GENERIC_ID, d->second->attach ? d->second->attach->vehicle : INVALID_GENERIC_ID, d->second->testLOS);
		if (internalID == INVALID_GENERIC_ID)
		{
			player.visibleTextLabels = player.internalTextLabels.size();
			break;
		}
		player.internalTextLabels.insert(std::make_pair(d->second->textLabelID, internalID));
		if (d->second->cell)
		{
			player.visibleCell->textLabels.insert(std::make_pair(d->second->textLabelID, d->second));
		}
	}
}

void Streamer::processActiveItems()
{
	if (!movingObjects.empty())
	{
		processMovingObjects();
	}
	if (!attachedAreas.empty())
	{
		processAttachedAreas();
	}
	if (!attachedObjects.empty())
	{
		processAttachedObjects();
	}
	if (!attachedTextLabels.empty())
	{
		processAttachedTextLabels();
	}
}

void Streamer::processMovingObjects()
{
	boost::chrono::steady_clock::time_point currentTime = boost::chrono::steady_clock::now();
	boost::unordered_set<Item::SharedObject>::iterator o = movingObjects.begin();
	while (o != movingObjects.end())
	{
		bool objectFinishedMoving = false;
		if ((*o)->move)
		{
			boost::chrono::duration<float, boost::milli> elapsedTime = currentTime - (*o)->move->time;
			if (elapsedTime.count() < (*o)->move->duration)
			{
				(*o)->position = (*o)->move->position.get<1>() + ((*o)->move->position.get<2>() * elapsedTime.count());
				if (((*o)->move->rotation.get<0>().maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
				{
					(*o)->rotation = (*o)->move->rotation.get<1>() + ((*o)->move->rotation.get<2>() * elapsedTime.count());
				}
			}
			else
			{
				(*o)->position = (*o)->move->position.get<0>();
				if (((*o)->move->rotation.get<0>().maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
				{
					(*o)->rotation = (*o)->move->rotation.get<0>();
				}
				(*o)->move.reset();
				objectMoveCallbacks.push_back((*o)->objectID);
				objectFinishedMoving = true;
			}
			if ((*o)->cell)
			{
				core->getGrid()->removeObject(*o, true);
			}
		}
		if (objectFinishedMoving)
		{
			o = movingObjects.erase(o);
		}
		else
		{
			++o;
		}
	}
}

void Streamer::processAttachedAreas()
{
	for (boost::unordered_set<Item::SharedArea>::iterator a = attachedAreas.begin(); a != attachedAreas.end(); ++a)
	{
		if ((*a)->attach)
		{
			bool adjust = false;
			if ((*a)->attach->object.get<0>() != INVALID_GENERIC_ID)
			{
				switch ((*a)->attach->object.get<1>())
				{
				case STREAMER_OBJECT_TYPE_GLOBAL:
				{
					adjust = GetObjectPos((*a)->attach->object.get<0>(), &(*a)->attach->position[0], &(*a)->attach->position[1], &(*a)->attach->position[2]);
					break;
				}
				case STREAMER_OBJECT_TYPE_PLAYER:
				{
					adjust = GetPlayerObjectPos((*a)->attach->object.get<2>(), (*a)->attach->object.get<0>(), &(*a)->attach->position[0], &(*a)->attach->position[1], &(*a)->attach->position[2]);
					break;
				}
				case STREAMER_OBJECT_TYPE_DYNAMIC:
				{
					boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find((*a)->attach->object.get<0>());
					if (o != core->getData()->objects.end())
					{
						(*a)->attach->position = o->second->position;
						adjust = true;
					}
					break;
				}
				}
			}
			else if ((*a)->attach->player != INVALID_GENERIC_ID)
			{
				adjust = GetPlayerPos((*a)->attach->player, &(*a)->attach->position[0], &(*a)->attach->position[1], &(*a)->attach->position[2]);
			}
			else if ((*a)->attach->vehicle != INVALID_GENERIC_ID)
			{
				adjust = GetVehiclePos((*a)->attach->vehicle, &(*a)->attach->position[0], &(*a)->attach->position[1], &(*a)->attach->position[2]);
			}
			if (adjust)
			{
				if ((*a)->cell)
				{
					core->getGrid()->removeArea(*a, true);
				}
			}
			else
			{
				(*a)->attach->position.fill(std::numeric_limits<float>::infinity());
			}
		}
	}
}

void Streamer::processAttachedObjects()
{
	for (boost::unordered_set<Item::SharedObject>::iterator o = attachedObjects.begin(); o != attachedObjects.end(); ++o)
	{
		if ((*o)->attach)
		{
			bool adjust = false;
			if ((*o)->attach->vehicle != INVALID_GENERIC_ID)
			{
				adjust = GetVehiclePos((*o)->attach->vehicle, &(*o)->attach->position[0], &(*o)->attach->position[1], &(*o)->attach->position[2]);
			}
			if (adjust)
			{
				if ((*o)->cell)
				{
					core->getGrid()->removeObject(*o, true);
				}
			}
			else
			{
				(*o)->attach->position.fill(std::numeric_limits<float>::infinity());
			}
		}
	}
}

void Streamer::processAttachedTextLabels()
{
	for (boost::unordered_set<Item::SharedTextLabel>::iterator t = attachedTextLabels.begin(); t != attachedTextLabels.end(); ++t)
	{
		bool adjust = false;
		if ((*t)->attach)
		{
			if ((*t)->attach->player != INVALID_GENERIC_ID)
			{
				adjust = GetPlayerPos((*t)->attach->player, &(*t)->attach->position[0], &(*t)->attach->position[1], &(*t)->attach->position[2]);
			}
			else if ((*t)->attach->vehicle != INVALID_GENERIC_ID)
			{
				adjust = GetVehiclePos((*t)->attach->vehicle, &(*t)->attach->position[0], &(*t)->attach->position[1], &(*t)->attach->position[2]);
			}
			if (adjust)
			{
				if ((*t)->cell)
				{
					core->getGrid()->removeTextLabel(*t, true);
				}
			}
			else
			{
				(*t)->attach->position.fill(std::numeric_limits<float>::infinity());
			}
		}
	}
}

using namespace Utility;

cell AMX_NATIVE_CALL Utility::hookedNative(AMX *amx, cell *params)
{
	return 1;
}

int Utility::checkInterfaceAndRegisterNatives(AMX *amx, AMX_NATIVE_INFO *amxNativeList)
{
	AMX_HEADER *amxHeader = reinterpret_cast<AMX_HEADER*>(amx->base);
	AMX_FUNCSTUBNT *amxNativeTable = reinterpret_cast<AMX_FUNCSTUBNT*>(amx->base + amxHeader->natives);
	int amxRegisterResult = amx_Register(amx, amxNativeList, -1);
	bool foundNatives = false;
	bool hookedNatives = false;
	int numberOfNatives = 0;
	amx_NumNatives(amx, &numberOfNatives);
	for (int i = 0; i < numberOfNatives; ++i)
	{
		char *name = reinterpret_cast<char*>(amx->base + amxNativeTable[i].nameofs);
		if (std::string(name).find("Streamer_") != std::string::npos)
		{
			foundNatives = true;
			if (!amxNativeTable[i].address)
			{
				sampgdk::logprintf("*** Streamer Plugin: Warning: Obsolete or invalid native \"%s\" found (script might need to be recompiled with the latest include file)", name);
				amxNativeTable[i].address = reinterpret_cast<cell>(hookedNative);
				hookedNatives = true;
			}
		}
	}
	if (foundNatives)
	{
		cell amxAddr = 0;
		int includeFileValue = 0;
		if (!amx_FindPubVar(amx, "Streamer_IncludeFileVersion", &amxAddr))
		{
			cell *amxPhysAddr = NULL;
			if (!amx_GetAddr(amx, amxAddr, &amxPhysAddr))
			{
				includeFileValue = static_cast<int>(*amxPhysAddr);
			}
		}
		if (includeFileValue != INCLUDE_FILE_VERSION)
		{
			std::ostringstream includeFileVersion;
			if (includeFileValue <= 0)
			{
				includeFileVersion << "unknown version";
			}
			else
			{
				includeFileVersion << std::hex << std::showbase << includeFileValue;
			}
			sampgdk::logprintf("*** Streamer Plugin: Warning: Include file version (%s) does not match plugin version (%#x) (script might need to be recompiled with the latest include file)", includeFileVersion.str().c_str(), INCLUDE_FILE_VERSION);
		}
	}
	if (hookedNatives)
	{
		amxRegisterResult = amx_Register(amx, amxNativeList, -1);
	}
	return amxRegisterResult;
}

void Utility::destroyAllItemsInInterface(AMX *amx)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.begin();
	while (o != core->getData()->objects.end())
	{
		if (o->second->amx == amx)
		{
			o = destroyObject(o);
		}
		else
		{
			++o;
		}
	}
	boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.begin();
	while (p != core->getData()->pickups.end())
	{
		if (p->second->amx == amx)
		{
			p = destroyPickup(p);
		}
		else
		{
			++p;
		}
	}
	boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin();
	while (c != core->getData()->checkpoints.end())
	{
		if (c->second->amx == amx)
		{
			c = destroyCheckpoint(c);
		}
		else
		{
			++c;
		}
	}
	boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin();
	while (r != core->getData()->raceCheckpoints.end())
	{
		if (r->second->amx == amx)
		{
			r = destroyRaceCheckpoint(r);
		}
		else
		{
			++r;
		}
	}
	boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.begin();
	while (m != core->getData()->mapIcons.end())
	{
		if (m->second->amx == amx)
		{
			m = destroyMapIcon(m);
		}
		else
		{
			++m;
		}
	}
	boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.begin();
	while (t != core->getData()->textLabels.end())
	{
		if (t->second->amx == amx)
		{
			t = destroyTextLabel(t);
		}
		else
		{
			++t;
		}
	}
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin();
	while (a != core->getData()->areas.end())
	{
		if (a->second->amx == amx)
		{
			a = destroyArea(a);
		}
		else
		{
			++a;
		}
	}
}

boost::unordered_map<int, Item::SharedArea>::iterator Utility::destroyArea(boost::unordered_map<int, Item::SharedArea>::iterator a)
{
	Item::Area::identifier.remove(a->first, core->getData()->areas.size());
	for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		boost::unordered_set<int>::iterator i = p->second.internalAreas.find(a->first);
		if (i != p->second.internalAreas.end())
		{
			core->getStreamer()->areaLeaveCallbacks.push_back(boost::make_tuple(a->first, p->first));
		}
		p->second.disabledAreas.erase(a->first);
		p->second.internalAreas.erase(a->first);
		p->second.visibleCell->areas.erase(a->first);
	}
	core->getGrid()->removeArea(a->second);
	return core->getData()->areas.erase(a);
}

boost::unordered_map<int, Item::SharedCheckpoint>::iterator Utility::destroyCheckpoint(boost::unordered_map<int, Item::SharedCheckpoint>::iterator c)
{
	Item::Checkpoint::identifier.remove(c->first, core->getData()->checkpoints.size());
	for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		if (p->second.visibleCheckpoint == c->first)
		{
			DisablePlayerCheckpoint(p->first);
			p->second.activeCheckpoint = 0;
			p->second.visibleCheckpoint = 0;
		}
		p->second.disabledCheckpoints.erase(c->first);
		p->second.visibleCell->checkpoints.erase(c->first);
	}
	core->getGrid()->removeCheckpoint(c->second);
	return core->getData()->checkpoints.erase(c);
}

boost::unordered_map<int, Item::SharedMapIcon>::iterator Utility::destroyMapIcon(boost::unordered_map<int, Item::SharedMapIcon>::iterator m)
{
	Item::MapIcon::identifier.remove(m->first, core->getData()->mapIcons.size());
	for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		boost::unordered_map<int, int>::iterator i = p->second.internalMapIcons.find(m->first);
		if (i != p->second.internalMapIcons.end())
		{
			RemovePlayerMapIcon(p->first, i->second);
			p->second.mapIconIdentifier.remove(i->second, p->second.internalMapIcons.size());
			p->second.internalMapIcons.quick_erase(i);
		}
		p->second.visibleCell->mapIcons.erase(m->first);
	}
	core->getGrid()->removeMapIcon(m->second);
	return core->getData()->mapIcons.erase(m);
}

boost::unordered_map<int, Item::SharedObject>::iterator Utility::destroyObject(boost::unordered_map<int, Item::SharedObject>::iterator o)
{
	Item::Object::identifier.remove(o->first, core->getData()->objects.size());
	for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
		if (i != p->second.internalObjects.end())
		{
			DestroyPlayerObject(p->first, i->second);
			p->second.internalObjects.quick_erase(i);
		}
		p->second.visibleCell->objects.erase(o->first);
	}
	core->getGrid()->removeObject(o->second);
	return core->getData()->objects.erase(o);
}

boost::unordered_map<int, Item::SharedPickup>::iterator Utility::destroyPickup(boost::unordered_map<int, Item::SharedPickup>::iterator p)
{
	Item::Pickup::identifier.remove(p->first, core->getData()->pickups.size());
	boost::unordered_map<int, int>::iterator i = core->getStreamer()->internalPickups.find(p->first);
	if (i != core->getStreamer()->internalPickups.end())
	{
		DestroyPickup(i->second);
		core->getStreamer()->internalPickups.quick_erase(i);
	}
	core->getGrid()->removePickup(p->second);
	return core->getData()->pickups.erase(p);
}

boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator Utility::destroyRaceCheckpoint(boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r)
{
	Item::RaceCheckpoint::identifier.remove(r->first, core->getData()->raceCheckpoints.size());
	for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		if (p->second.visibleRaceCheckpoint == r->first)
		{
			DisablePlayerRaceCheckpoint(p->first);
			p->second.activeRaceCheckpoint = 0;
			p->second.visibleRaceCheckpoint = 0;
		}
		p->second.disabledRaceCheckpoints.erase(r->first);
		p->second.visibleCell->raceCheckpoints.erase(r->first);
	}
	core->getGrid()->removeRaceCheckpoint(r->second);
	return core->getData()->raceCheckpoints.erase(r);
}

boost::unordered_map<int, Item::SharedTextLabel>::iterator Utility::destroyTextLabel(boost::unordered_map<int, Item::SharedTextLabel>::iterator t)
{
	Item::TextLabel::identifier.remove(t->first, core->getData()->textLabels.size());
	for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(t->first);
		if (i != p->second.internalTextLabels.end())
		{
			DeletePlayer3DTextLabel(p->first, i->second);
			p->second.internalTextLabels.quick_erase(i);
		}
		p->second.visibleCell->textLabels.erase(t->first);
	}
	core->getGrid()->removeTextLabel(t->second);
	return core->getData()->textLabels.erase(t);
}

bool Utility::isPointInArea(const Eigen::Vector3f &point, const Item::SharedArea &area)
{
	switch (area->type)
	{
	case STREAMER_AREA_TYPE_CIRCLE:
	{
		if (area->attach)
		{
			if (boost::geometry::comparable_distance(Eigen::Vector2f(point[0], point[1]), Eigen::Vector2f(area->attach->position[0], area->attach->position[1])) <= area->size)
			{
				return true;
			}
		}
		else
		{
			if (boost::geometry::comparable_distance(Eigen::Vector2f(point[0], point[1]), boost::get<Eigen::Vector2f>(area->position)) <= area->size)
			{
				return true;
			}
		}
		return false;
	}
	case STREAMER_AREA_TYPE_CYLINDER:
	{
		if (point[2] >= area->height[0] && point[2] <= area->height[1])
		{
			if (boost::geometry::comparable_distance(Eigen::Vector2f(point[0], point[1]), boost::get<Eigen::Vector2f>(area->position)) <= area->size)
			{
				return true;
			}
		}
		return false;
	}
	case STREAMER_AREA_TYPE_SPHERE:
	{
		if (area->attach)
		{
			if (boost::geometry::comparable_distance(point, area->attach->position) <= area->size)
			{
				return true;
			}
		}
		else
		{
			if (boost::geometry::comparable_distance(point, boost::get<Eigen::Vector3f>(area->position)) <= area->size)
			{
				return true;
			}
		}
		return false;
	}
	case STREAMER_AREA_TYPE_RECTANGLE:
	{
		return boost::geometry::covered_by(Eigen::Vector2f(point[0], point[1]), boost::get<Box2D>(area->position));
	}
	case STREAMER_AREA_TYPE_CUBOID:
	{
		return boost::geometry::covered_by(point, boost::get<Box3D>(area->position));
	}
	case STREAMER_AREA_TYPE_POLYGON:
	{
		if (point[2] >= area->height[0] && point[2] <= area->height[1])
		{
			return boost::geometry::covered_by(Eigen::Vector2f(point[0], point[1]), boost::get<Polygon2D>(area->position));
		}
		return false;
	}
	}
	return false;
}

void Utility::convertArrayToPolygon(AMX *amx, cell input, cell size, Polygon2D &polygon)
{
	cell *array = NULL;
	std::vector<Eigen::Vector2f> points;
	amx_GetAddr(amx, input, &array);
	for (std::size_t i = 0; i < static_cast<std::size_t>(size); i += 2)
	{
		points.push_back(Eigen::Vector2f(amx_ctof(array[i]), amx_ctof(array[i + 1])));
	}
	boost::geometry::assign_points(polygon, points);
	boost::geometry::correct(polygon);
}

bool Utility::convertPolygonToArray(AMX *amx, cell output, cell size, Polygon2D &polygon)
{
	cell *array = NULL;
	std::size_t i = 0;
	amx_GetAddr(amx, output, &array);
	for (std::vector<Eigen::Vector2f>::iterator p = polygon.outer().begin(); p != polygon.outer().end(); ++p)
	{
		if ((i + 1) >= static_cast<std::size_t>(size))
		{
			return false;
		}
		array[i++] = amx_ftoc(p->data()[0]);
		array[i++] = amx_ftoc(p->data()[1]);
	}
	return true;
}

std::string Utility::convertNativeStringToString(AMX *amx, cell input)
{
	char *string = NULL;
	amx_StrParam(amx, input, string);
	return string ? string : "";
}

void Utility::convertStringToNativeString(AMX *amx, cell output, cell size, std::string string)
{
	cell *address = NULL;
	amx_GetAddr(amx, output, &address);
	amx_SetString(address, string.c_str(), 0, 0, static_cast<size_t>(size));
}

void Utility::storeFloatInNative(AMX *amx, cell output, float value)
{
	cell *address;
	amx_GetAddr(amx, output, &address);
	*address = amx_ftoc(value);
}

void Utility::storeIntegerInNative(AMX *amx, cell output, int value)
{
	cell *address;
	amx_GetAddr(amx, output, &address);
	*address = value;
}

/*
* Copyright (C) 2014 Incognito
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

Identifier Item::Area::identifier;
Identifier Item::Checkpoint::identifier;
Identifier Item::MapIcon::identifier;
Identifier Item::Object::identifier;
Identifier Item::Pickup::identifier;
Identifier Item::RaceCheckpoint::identifier;
Identifier Item::TextLabel::identifier;

Item::Area::Area() : references(0) {}
Item::Area::Attach::Attach() : references(0) {}
Item::Checkpoint::Checkpoint() : references(0) {}
Item::MapIcon::MapIcon() : references(0) {}
Item::Object::Object() : references(0) {}
Item::Object::Attach::Attach() : references(0) {}
Item::Object::Material::Main::Main() : references(0) {}
Item::Object::Material::Text::Text() : references(0) {}
Item::Object::Move::Move() : references(0) {}
Item::Pickup::Pickup() : references(0) {}
Item::RaceCheckpoint::RaceCheckpoint() : references(0) {}
Item::TextLabel::TextLabel() : references(0) {}
Item::TextLabel::Attach::Attach() : references(0) {}

Identifier::Identifier()
{
	highestID = 0;
}

int Identifier::get()
{
	int id = 0;
	if (!removedIDs.empty())
	{
		id = removedIDs.top();
		removedIDs.pop();
	}
	else
	{
		id = ++highestID;
	}
	return id;
}

void Identifier::remove(int id, std::size_t remaining)
{
	if (remaining > 1)
	{
		removedIDs.push(id);
	}
	else
	{
		reset();
	}
}

void Identifier::reset()
{
	highestID = 0;
	removedIDs = std::priority_queue<int, std::vector<int>, std::greater<int> >();
}

Grid::Grid()
{
	cellDistance = 360000.0f;
	cellSize = 300.0f;
	globalCell = SharedCell(new Cell());
	calculateTranslationMatrix();
}

void Grid::addArea(const Item::SharedArea &area)
{
	if (area->size > cellDistance)
	{
		globalCell->areas.insert(std::make_pair(area->areaID, area));
		area->cell.reset();
	}
	else
	{
		Eigen::Vector2f position = Eigen::Vector2f::Zero();
		switch (area->type)
		{
		case STREAMER_AREA_TYPE_CIRCLE:
		case STREAMER_AREA_TYPE_CYLINDER:
		{
			if (area->attach)
			{
				position = Eigen::Vector2f(area->attach->position[0], area->attach->position[1]);
			}
			else
			{
				position = boost::get<Eigen::Vector2f>(area->position);
			}
			break;
		}
		case STREAMER_AREA_TYPE_SPHERE:
		{
			if (area->attach)
			{
				position = Eigen::Vector2f(area->attach->position[0], area->attach->position[1]);
			}
			else
			{
				position = Eigen::Vector2f(boost::get<Eigen::Vector3f>(area->position)[0], boost::get<Eigen::Vector3f>(area->position)[1]);
			}
			break;
		}
		case STREAMER_AREA_TYPE_RECTANGLE:
		{
			boost::geometry::centroid(boost::get<Box2D>(area->position), position);
			break;
		}
		case STREAMER_AREA_TYPE_CUBOID:
		{
			Eigen::Vector3f centroid = boost::geometry::return_centroid<Eigen::Vector3f>(boost::get<Box3D>(area->position));
			position = Eigen::Vector2f(centroid[0], centroid[1]);
			break;
		}
		case STREAMER_AREA_TYPE_POLYGON:
		{
			boost::geometry::centroid(boost::get<Polygon2D>(area->position), position);
			break;
		}
		}
		CellID cellID = getCellID(position);
		cells[cellID]->areas.insert(std::make_pair(area->areaID, area));
		area->cell = cells[cellID];
	}
}

void Grid::addCheckpoint(const Item::SharedCheckpoint &checkpoint)
{
	if (checkpoint->streamDistance > cellDistance)
	{
		globalCell->checkpoints.insert(std::make_pair(checkpoint->checkpointID, checkpoint));
		checkpoint->cell.reset();
	}
	else
	{
		CellID cellID = getCellID(Eigen::Vector2f(checkpoint->position[0], checkpoint->position[1]));
		cells[cellID]->checkpoints.insert(std::make_pair(checkpoint->checkpointID, checkpoint));
		checkpoint->cell = cells[cellID];
	}
}

void Grid::addMapIcon(const Item::SharedMapIcon &mapIcon)
{
	if (mapIcon->streamDistance > cellDistance)
	{
		globalCell->mapIcons.insert(std::make_pair(mapIcon->mapIconID, mapIcon));
		mapIcon->cell.reset();
	}
	else
	{
		CellID cellID = getCellID(Eigen::Vector2f(mapIcon->position[0], mapIcon->position[1]));
		cells[cellID]->mapIcons.insert(std::make_pair(mapIcon->mapIconID, mapIcon));
		mapIcon->cell = cells[cellID];
	}
}

void Grid::addObject(const Item::SharedObject &object)
{
	if (object->streamDistance > cellDistance)
	{
		globalCell->objects.insert(std::make_pair(object->objectID, object));
		object->cell.reset();
	}
	else
	{
		Eigen::Vector2f position = Eigen::Vector2f::Zero();
		if (object->attach)
		{
			position = Eigen::Vector2f(object->attach->position[0], object->attach->position[1]);
		}
		else
		{
			position = Eigen::Vector2f(object->position[0], object->position[1]);
		}
		CellID cellID = getCellID(Eigen::Vector2f(position[0], position[1]));
		cells[cellID]->objects.insert(std::make_pair(object->objectID, object));
		object->cell = cells[cellID];
	}
}

void Grid::addPickup(const Item::SharedPickup &pickup)
{
	if (pickup->streamDistance > cellDistance)
	{
		globalCell->pickups.insert(std::make_pair(pickup->pickupID, pickup));
		pickup->cell.reset();
	}
	else
	{
		CellID cellID = getCellID(Eigen::Vector2f(pickup->position[0], pickup->position[1]));
		cells[cellID]->pickups.insert(std::make_pair(pickup->pickupID, pickup));
		pickup->cell = cells[cellID];
	}
}

void Grid::addRaceCheckpoint(const Item::SharedRaceCheckpoint &raceCheckpoint)
{
	if (raceCheckpoint->streamDistance > cellDistance)
	{
		globalCell->raceCheckpoints.insert(std::make_pair(raceCheckpoint->raceCheckpointID, raceCheckpoint));
		raceCheckpoint->cell.reset();
	}
	else
	{
		CellID cellID = getCellID(Eigen::Vector2f(raceCheckpoint->position[0], raceCheckpoint->position[1]));
		cells[cellID]->raceCheckpoints.insert(std::make_pair(raceCheckpoint->raceCheckpointID, raceCheckpoint));
		raceCheckpoint->cell = cells[cellID];
	}
}

void Grid::addTextLabel(const Item::SharedTextLabel &textLabel)
{
	if (textLabel->streamDistance > cellDistance)
	{
		globalCell->textLabels.insert(std::make_pair(textLabel->textLabelID, textLabel));
		textLabel->cell.reset();
	}
	else
	{
		Eigen::Vector2f position = Eigen::Vector2f::Zero();
		if (textLabel->attach)
		{
			position = Eigen::Vector2f(textLabel->attach->position[0], textLabel->attach->position[1]);
		}
		else
		{
			position = Eigen::Vector2f(textLabel->position[0], textLabel->position[1]);;
		}
		CellID cellID = getCellID(position);
		cells[cellID]->textLabels.insert(std::make_pair(textLabel->textLabelID, textLabel));
		textLabel->cell = cells[cellID];
	}
}

void Grid::rebuildGrid()
{
	cells.clear();
	globalCell = SharedCell(new Cell());
	calculateTranslationMatrix();
	for (boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
	{
		addArea(a->second);
	}
	for (boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin(); c != core->getData()->checkpoints.end(); ++c)
	{
		addCheckpoint(c->second);
	}
	for (boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.begin(); m != core->getData()->mapIcons.end(); ++m)
	{
		addMapIcon(m->second);
	}
	for (boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.begin(); o != core->getData()->objects.end(); ++o)
	{
		addObject(o->second);
	}
	for (boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.begin(); p != core->getData()->pickups.end(); ++p)
	{
		addPickup(p->second);
	}
	for (boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin(); r != core->getData()->raceCheckpoints.end(); ++r)
	{
		addRaceCheckpoint(r->second);
	}
	for (boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.begin(); t != core->getData()->textLabels.end(); ++t)
	{
		addTextLabel(t->second);
	}
}

void Grid::removeArea(const Item::SharedArea &area, bool reassign)
{
	bool found = false;
	if (area->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(area->cell->cellID);
		if (c != cells.end())
		{
			boost::unordered_map<int, Item::SharedArea>::iterator a = c->second->areas.find(area->areaID);
			if (a != c->second->areas.end())
			{
				c->second->areas.quick_erase(a);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Item::SharedArea>::iterator a = globalCell->areas.find(area->areaID);
		if (a != globalCell->areas.end())
		{
			globalCell->areas.quick_erase(a);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addArea(area);
		}
		else
		{
			if (area->attach)
			{
				core->getStreamer()->attachedAreas.erase(area);
			}
		}
	}
}

void Grid::removeCheckpoint(const Item::SharedCheckpoint &checkpoint, bool reassign)
{
	bool found = false;
	if (checkpoint->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(checkpoint->cell->cellID);
		if (c != cells.end())
		{
			boost::unordered_map<int, Item::SharedCheckpoint>::iterator d = c->second->checkpoints.find(checkpoint->checkpointID);
			if (d != c->second->checkpoints.end())
			{
				c->second->checkpoints.quick_erase(d);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = globalCell->checkpoints.find(checkpoint->checkpointID);
		if (c != globalCell->checkpoints.end())
		{
			globalCell->checkpoints.quick_erase(c);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addCheckpoint(checkpoint);
		}
	}
}

void Grid::removeMapIcon(const Item::SharedMapIcon &mapIcon, bool reassign)
{
	bool found = false;
	if (mapIcon->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(mapIcon->cell->cellID);
		if (c != cells.end())
		{
			boost::unordered_map<int, Item::SharedMapIcon>::iterator m = c->second->mapIcons.find(mapIcon->mapIconID);
			if (m != c->second->mapIcons.end())
			{
				c->second->mapIcons.quick_erase(m);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = globalCell->mapIcons.find(mapIcon->mapIconID);
		if (m != globalCell->mapIcons.end())
		{
			globalCell->mapIcons.quick_erase(m);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addMapIcon(mapIcon);
		}
	}
}

void Grid::removeObject(const Item::SharedObject &object, bool reassign)
{
	bool found = false;
	if (object->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(object->cell->cellID);
		if (c != cells.end())
		{
			boost::unordered_map<int, Item::SharedObject>::iterator o = c->second->objects.find(object->objectID);
			if (o != c->second->objects.end())
			{
				c->second->objects.quick_erase(o);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = globalCell->objects.find(object->objectID);
		if (o != globalCell->objects.end())
		{
			globalCell->objects.quick_erase(o);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addObject(object);
		}
		else
		{
			if (object->attach)
			{
				core->getStreamer()->attachedObjects.erase(object);
			}
			if (object->move)
			{
				core->getStreamer()->movingObjects.erase(object);
			}
		}
	}
}

void Grid::removePickup(const Item::SharedPickup &pickup, bool reassign)
{
	bool found = false;
	if (pickup->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(pickup->cell->cellID);
		if (c != cells.end())
		{
			boost::unordered_map<int, Item::SharedPickup>::iterator p = c->second->pickups.find(pickup->pickupID);
			if (p != c->second->pickups.end())
			{
				c->second->pickups.quick_erase(p);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = globalCell->pickups.find(pickup->pickupID);
		if (p != globalCell->pickups.end())
		{
			globalCell->pickups.quick_erase(p);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addPickup(pickup);
		}
	}
}

void Grid::removeRaceCheckpoint(const Item::SharedRaceCheckpoint &raceCheckpoint, bool reassign)
{
	bool found = false;
	if (raceCheckpoint->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(raceCheckpoint->cell->cellID);
		if (c != cells.end())
		{
			boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = c->second->raceCheckpoints.find(raceCheckpoint->raceCheckpointID);
			if (r != c->second->raceCheckpoints.end())
			{
				c->second->raceCheckpoints.quick_erase(r);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = globalCell->raceCheckpoints.find(raceCheckpoint->raceCheckpointID);
		if (r != globalCell->raceCheckpoints.end())
		{
			globalCell->raceCheckpoints.quick_erase(r);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addRaceCheckpoint(raceCheckpoint);
		}
	}
}

void Grid::removeTextLabel(const Item::SharedTextLabel &textLabel, bool reassign)
{
	bool found = false;
	if (textLabel->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(textLabel->cell->cellID);
		if (c != cells.end())
		{
			boost::unordered_map<int, Item::SharedTextLabel>::iterator t = c->second->textLabels.find(textLabel->textLabelID);
			if (t != c->second->textLabels.end())
			{
				c->second->textLabels.quick_erase(t);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = globalCell->textLabels.find(textLabel->textLabelID);
		if (t != globalCell->textLabels.end())
		{
			globalCell->textLabels.quick_erase(t);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addTextLabel(textLabel);
		}
		else
		{
			if (textLabel->attach)
			{
				core->getStreamer()->attachedTextLabels.erase(textLabel);
			}
		}
	}
}

CellID Grid::getCellID(const Eigen::Vector2f &position, bool insert)
{
	static Box2D box;
	box.min_corner()[0] = std::floor((position[0] / cellSize)) * cellSize;
	box.min_corner()[1] = std::floor((position[1] / cellSize)) * cellSize;
	box.max_corner()[0] = box.min_corner()[0] + cellSize;
	box.max_corner()[1] = box.min_corner()[1] + cellSize;
	Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(box);
	CellID cellID = std::make_pair(static_cast<int>(centroid[0]), static_cast<int>(centroid[1]));
	if (insert)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(cellID);
		if (c == cells.end())
		{
			SharedCell cell(new Cell(cellID));
			cells[cellID] = cell;
		}
	}
	return cellID;
}

void Grid::processDiscoveredCells(Player &player, std::vector<SharedCell> &playerCells, const boost::unordered_set<CellID> &discoveredCells)
{
	playerCells.push_back(SharedCell(new Cell()));;
	if (player.enabledItems[STREAMER_TYPE_OBJECT])
	{
		boost::unordered_map<int, Item::SharedObject>::iterator o = player.visibleCell->objects.begin();
		while (o != player.visibleCell->objects.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(o->second->cell->cellID);
			if (d != discoveredCells.end())
			{
				o = player.visibleCell->objects.erase(o);
			}
			else
			{
				++o;
			}
		}
		playerCells.back()->objects.swap(player.visibleCell->objects);
	}
	if (player.enabledItems[STREAMER_TYPE_CP])
	{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = player.visibleCell->checkpoints.begin();
		while (c != player.visibleCell->checkpoints.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(c->second->cell->cellID);
			if (d != discoveredCells.end())
			{
				c = player.visibleCell->checkpoints.erase(c);
			}
			else
			{
				++c;
			}
		}
		playerCells.back()->checkpoints.swap(player.visibleCell->checkpoints);
	}
	if (player.enabledItems[STREAMER_TYPE_RACE_CP])
	{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator t = player.visibleCell->raceCheckpoints.begin();
		while (t != player.visibleCell->raceCheckpoints.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(t->second->cell->cellID);
			if (d != discoveredCells.end())
			{
				t = player.visibleCell->raceCheckpoints.erase(t);
			}
			else
			{
				++t;
			}
		}
		playerCells.back()->raceCheckpoints.swap(player.visibleCell->raceCheckpoints);
	}
	if (player.enabledItems[STREAMER_TYPE_MAP_ICON])
	{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = player.visibleCell->mapIcons.begin();
		while (m != player.visibleCell->mapIcons.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(m->second->cell->cellID);
			if (d != discoveredCells.end())
			{
				m = player.visibleCell->mapIcons.erase(m);
			}
			else
			{
				++m;
			}
		}
		playerCells.back()->mapIcons.swap(player.visibleCell->mapIcons);
	}
	if (player.enabledItems[STREAMER_TYPE_3D_TEXT_LABEL])
	{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = player.visibleCell->textLabels.begin();
		while (t != player.visibleCell->textLabels.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(t->second->cell->cellID);
			if (d != discoveredCells.end())
			{
				t = player.visibleCell->textLabels.erase(t);
			}
			else
			{
				++t;
			}
		}
		playerCells.back()->textLabels.swap(player.visibleCell->textLabels);
	}
	if (player.enabledItems[STREAMER_TYPE_AREA])
	{
		boost::unordered_map<int, Item::SharedArea>::iterator a = player.visibleCell->areas.begin();
		while (a != player.visibleCell->areas.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(a->second->cell->cellID);
			if (d != discoveredCells.end())
			{
				a = player.visibleCell->areas.erase(a);
			}
			else
			{
				++a;
			}
		}
		playerCells.back()->areas.swap(player.visibleCell->areas);
	}
}

void Grid::findAllCells(Player &player, std::vector<SharedCell> &playerCells)
{
	boost::unordered_set<CellID> discoveredCells;
	for (int i = 0; i < translationMatrix.cols(); ++i)
	{
		Eigen::Vector2f position = Eigen::Vector2f(player.position[0], player.position[1]) + translationMatrix.col(i);
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(getCellID(position, false));
		if (c != cells.end())
		{
			discoveredCells.insert(c->first);
			playerCells.push_back(c->second);
		}
	}
	processDiscoveredCells(player, playerCells, discoveredCells);
	playerCells.push_back(globalCell);
}

void Grid::findMinimalCells(Player &player, std::vector<SharedCell> &playerCells)
{
	for (int i = 0; i < translationMatrix.cols(); ++i)
	{
		Eigen::Vector2f position = Eigen::Vector2f(player.position[0], player.position[1]) + translationMatrix.col(i);
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(getCellID(position, false));
		if (c != cells.end())
		{
			playerCells.push_back(c->second);
		}
	}
	playerCells.push_back(globalCell);
}

Data::Data()
{
	maxAreas = std::numeric_limits<int>::max();
	maxCheckpoints = std::numeric_limits<int>::max();
	maxMapIcons = std::numeric_limits<int>::max();
	maxObjects = std::numeric_limits<int>::max();
	maxPickups = std::numeric_limits<int>::max();
	maxRaceCheckpoints = std::numeric_limits<int>::max();
	maxTextLabels = std::numeric_limits<int>::max();
}

std::size_t Data::getMaxItems(int type)
{
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		return maxObjects;
	}
	case STREAMER_TYPE_PICKUP:
	{
		return maxPickups;
	}
	case STREAMER_TYPE_CP:
	{
		return maxCheckpoints;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		return maxRaceCheckpoints;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		return maxMapIcons;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		return maxTextLabels;
	}
	case STREAMER_TYPE_AREA:
	{
		return maxAreas;
	}
	}
	return 0;
}

bool Data::setMaxItems(int type, std::size_t value)
{
	switch (type)
	{
	case STREAMER_TYPE_OBJECT:
	{
		maxObjects = value;
		return true;
	}
	case STREAMER_TYPE_PICKUP:
	{
		maxPickups = value;
		return true;
	}
	case STREAMER_TYPE_CP:
	{
		maxCheckpoints = value;
		return true;
	}
	case STREAMER_TYPE_RACE_CP:
	{
		maxRaceCheckpoints = value;
		return true;
	}
	case STREAMER_TYPE_MAP_ICON:
	{
		maxMapIcons = value;
		return true;
	}
	case STREAMER_TYPE_3D_TEXT_LABEL:
	{
		maxTextLabels = value;
		return true;
	}
	case STREAMER_TYPE_AREA:
	{
		maxAreas = value;
		return true;
	}
	}
	return false;
}

boost::scoped_ptr<Core> core;

Core::Core()
{
	data.reset(new Data);
	grid.reset(new Grid);
	streamer.reset(new Streamer);
}


Cell::Cell() : references(0) {}
Cell::Cell(CellID cellID) : cellID(cellID), references(0) {}

namespace StreamerLibrary
{
	bool OnPlayerConnect(int playerid)
	{
		if (playerid >= 0 && playerid < MAX_PLAYERS)
		{
			boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
			if (p == core->getData()->players.end())
			{
				Player player(playerid);
				core->getData()->players.insert(std::make_pair(playerid, player));
			}
		}
		return true;
	}

	bool OnPlayerDisconnect(int playerid, int reason)
	{
		core->getData()->players.erase(playerid);
		return true;
	}

	bool OnPlayerEnterCheckpoint(int playerid)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			if (p->second.activeCheckpoint != p->second.visibleCheckpoint)
			{
				int checkpointid = p->second.visibleCheckpoint;
				p->second.activeCheckpoint = checkpointid;
				OnPlayerEnterDynamicCP(playerid, checkpointid);
			}
		}
		return true;
	}

	bool OnPlayerLeaveCheckpoint(int playerid)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			if (p->second.activeCheckpoint == p->second.visibleCheckpoint)
			{
				int checkpointid = p->second.activeCheckpoint;
				p->second.activeCheckpoint = 0;
				OnPlayerLeaveDynamicCP(playerid, checkpointid);
			}
		}
		return true;
	}

	bool OnPlayerEnterRaceCheckpoint(int playerid)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			if (p->second.activeRaceCheckpoint != p->second.visibleRaceCheckpoint)
			{
				int checkpointid = p->second.visibleRaceCheckpoint;
				p->second.activeRaceCheckpoint = checkpointid;
				OnPlayerEnterDynamicRaceCP(playerid, checkpointid);
			}
		}
		return true;
	}

	bool OnPlayerLeaveRaceCheckpoint(int playerid)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			if (p->second.activeRaceCheckpoint == p->second.visibleRaceCheckpoint)
			{
				int checkpointid = p->second.activeRaceCheckpoint;
				p->second.activeRaceCheckpoint = 0;
				OnPlayerLeaveDynamicRaceCP(playerid, checkpointid);
			}
		}
		return true;
	}

	bool OnPlayerPickUpPickup(int playerid, int pickupid)
	{
		for (boost::unordered_map<int, int>::iterator i = core->getStreamer()->internalPickups.begin(); i != core->getStreamer()->internalPickups.end(); ++i)
		{
			if (i->second == pickupid)
			{
				int pickupid = i->first;
				OnPlayerPickUpDynamicPickup(playerid, pickupid);
				break;
			}
		}
		return true;
	}

	bool OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ)
	{
		if (playerobject)
		{
			boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
			if (p != core->getData()->players.end())
			{
				for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i)
				{
					if (i->second == objectid)
					{
						int objectid = i->first;
						OnPlayerEditDynamicObject(playerid, objectid, response, fX, fY, fZ, fRotX, fRotY, fRotZ);
						break;
					}
				}
			}
		}
		return true;
	}

	bool OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float x, float y, float z)
	{
		if (type == SELECT_OBJECT_PLAYER_OBJECT)
		{
			boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
			if (p != core->getData()->players.end())
			{
				for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i)
				{
					if (i->second == objectid)
					{
						int objectid = i->first;
						OnPlayerSelectDynamicObject(playerid, objectid, modelid, x, y, z);
						break;
					}
				}
			}
		}
		return true;
	}

	bool OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float x, float y, float z)
	{
		if (hittype == BULLET_HIT_TYPE_PLAYER_OBJECT)
		{
			boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
			if (p != core->getData()->players.end())
			{
				for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i)
				{
					if (i->second == hitid)
					{
						int objectid = i->first;
						OnPlayerShootDynamicObject(playerid, weaponid, objectid, x, y, z);
						break;
					}
				}
			}
		}
		return true;
	}
};

namespace StreamerLibrary
{
	int Streamer_ProcessActiveItems()
	{
		core->getStreamer()->processActiveItems();
		return 1;
	}

	int Streamer_ToggleIdleUpdate(int playerid, bool toggle)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			p->second.updateWhenIdle = toggle;
			return 1;
		}
		return 0;
	}

	int Streamer_IsToggleIdleUpdate(int playerid)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			return static_cast<cell>(p->second.updateWhenIdle != 0);
		}
		return 0;
	}

	int Streamer_ToggleItemUpdate(int playerid, int type, bool toggle)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			if (type >= 0 && type < STREAMER_MAX_TYPES)
			{
				p->second.enabledItems.set(type, toggle);
				return 1;
			}
		}
		return 0;
	}

	int Streamer_IsToggleItemUpdate(int playerid, int type)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			if (type >= 0 && type < STREAMER_MAX_TYPES)
			{
				return static_cast<cell>(p->second.enabledItems.test(type) != 0);
			}
		}
		return 0;
	}

	int Streamer_Update(int playerid)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			core->getStreamer()->startManualUpdate(p->second, true);
			return 1;
		}
		return 0;
	}

	int Streamer_UpdateEx(int playerid, float x, float y, float z, int worldid, int interiorid)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			p->second.position = Eigen::Vector3f(x, y, z);
			if (worldid >= 0)
			{
				p->second.worldID = worldid;
			}
			else
			{
				p->second.worldID = GetPlayerVirtualWorld(p->first);
			}
			if (interiorid >= 0)
			{
				p->second.interiorID = interiorid;
			}
			else
			{
				p->second.interiorID = GetPlayerInterior(p->first);
			}
			core->getStreamer()->startManualUpdate(p->second, false);
			return 1;
		}
		return 0;
	}
}

namespace StreamerLibrary
{
	int CreateDynamic3DTextLabel(std::string text, int color, float x, float y, float z, float drawdistance, int attachedplayer, int attachedvehicle, int testlos, int worldid, int interiorid, int playerid, float streamdistance)
	{

		if (core->getData()->getMaxItems(STREAMER_TYPE_3D_TEXT_LABEL) == core->getData()->textLabels.size())
		{
			return 0;
		}
		int textLabelID = Item::TextLabel::identifier.get();
		Item::SharedTextLabel textLabel(new Item::TextLabel);
		textLabel->amx = &StreamerAMX;
		textLabel->textLabelID = textLabelID;
		textLabel->text = text;
		textLabel->color = color;
		textLabel->position = Eigen::Vector3f(x, y, z);
		textLabel->drawDistance = drawdistance;
		if (attachedplayer != INVALID_GENERIC_ID || attachedvehicle != INVALID_GENERIC_ID)
		{
			textLabel->attach = boost::intrusive_ptr<Item::TextLabel::Attach>(new Item::TextLabel::Attach);
			textLabel->attach->player = attachedplayer;
			textLabel->attach->vehicle = attachedvehicle;
			if (textLabel->position.cwiseAbs().maxCoeff() > 50.0f)
			{
				textLabel->position.setZero();
			}
			core->getStreamer()->attachedTextLabels.insert(textLabel);
		}
		textLabel->testLOS = testlos != 0;
		Utility::addToContainer(textLabel->worlds, worldid);
		Utility::addToContainer(textLabel->interiors, interiorid);
		Utility::addToContainer(textLabel->players, playerid);
		textLabel->streamDistance = streamdistance * streamdistance;
		core->getGrid()->addTextLabel(textLabel);
		core->getData()->textLabels.insert(std::make_pair(textLabelID, textLabel));
		return static_cast<cell>(textLabelID);
	}

	int DestroyDynamic3DTextLabel(int id)
	{

		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end())
		{
			Utility::destroyTextLabel(t);
			return 1;
		}
		return 0;
	}

	int IsValidDynamic3DTextLabel(int id)
	{

		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end())
		{
			return 1;
		}
		return 0;
	}

	int GetDynamic3DTextLabelText(int id, std::string &text)
	{

		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end())
		{
			text = t->second->text;
			return 1;
		}
		return 0;
	}

	int UpdateDynamic3DTextLabelText(int id, int color, std::string text)
	{

		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
		if (t != core->getData()->textLabels.end())
		{
			t->second->color = color;
			t->second->text = text;
			for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
			{
				boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(t->first);
				if (i != p->second.internalTextLabels.end())
				{
					UpdatePlayer3DTextLabelText(p->first, i->second, t->second->color, t->second->text.c_str());
				}
			}
			return 1;
		}
		return 0;
	}
}

namespace StreamerLibrary
{
	int Streamer_GetTickRate()
	{
		return static_cast<cell>(core->getStreamer()->getTickRate());
	}

	int Streamer_SetTickRate(int rate)
	{

		if (rate > 0)
		{
			core->getStreamer()->setTickRate(rate);
			return 1;
		}
		return 0;
	}

	int Streamer_GetMaxItems(int type)
	{

		return static_cast<cell>(core->getData()->getMaxItems(type));
	}

	int Streamer_SetMaxItems(int type, int items)
	{

		return static_cast<cell>(core->getData()->setMaxItems(type, items) != 0);
	}

	int Streamer_GetVisibleItems(int type)
	{

		return static_cast<cell>(core->getStreamer()->getVisibleItems(type));
	}

	int Streamer_SetVisibleItems(int type, int items)
	{

		return static_cast<cell>(core->getStreamer()->setVisibleItems(type, items) != 0);
	}

	int Streamer_GetCellDistance(float &distance)
	{
		distance = core->getGrid()->getCellDistance();
		return 1;
	}

	int Streamer_SetCellDistance(float distance)
	{

		core->getGrid()->setCellDistance(distance * distance);
		core->getGrid()->rebuildGrid();
		return 1;
	}

	int Streamer_GetCellSize(float &size)
	{

		size = core->getGrid()->getCellSize();
		return 1;
	}

	int Streamer_SetCellSize(float size)
	{

		core->getGrid()->setCellSize(size);
		core->getGrid()->rebuildGrid();
		return 1;
	}
};

namespace StreamerLibrary
{
	int CreateDynamicRaceCP(int type, float x, float y, float z, float nextx, float nexty, float nextz, float size, int worldid, int interiorid, int playerid, float streamdistance)
	{

		if (core->getData()->getMaxItems(STREAMER_TYPE_RACE_CP) == core->getData()->raceCheckpoints.size())
		{
			return 0;
		}
		int raceCheckpointID = Item::RaceCheckpoint::identifier.get();
		Item::SharedRaceCheckpoint raceCheckpoint(new Item::RaceCheckpoint);
		raceCheckpoint->amx = &StreamerAMX;
		raceCheckpoint->raceCheckpointID = raceCheckpointID;
		raceCheckpoint->type = type;
		raceCheckpoint->position = Eigen::Vector3f(x, y, z);
		raceCheckpoint->next = Eigen::Vector3f(nextx, nexty, nextz);
		raceCheckpoint->size = size;
		Utility::addToContainer(raceCheckpoint->worlds, worldid);
		Utility::addToContainer(raceCheckpoint->interiors, interiorid);
		Utility::addToContainer(raceCheckpoint->players, playerid);
		raceCheckpoint->streamDistance = streamdistance * streamdistance;
		core->getGrid()->addRaceCheckpoint(raceCheckpoint);
		core->getData()->raceCheckpoints.insert(std::make_pair(raceCheckpointID, raceCheckpoint));
		return static_cast<cell>(raceCheckpointID);
	}

	int DestroyDynamicRaceCP(int checkpointid)
	{

		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(checkpointid);
		if (r != core->getData()->raceCheckpoints.end())
		{
			Utility::destroyRaceCheckpoint(r);
			return 1;
		}
		return 0;
	}

	bool IsValidDynamicRaceCP(int checkpointid)
	{

		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(checkpointid);
		if (r != core->getData()->raceCheckpoints.end())
		{
			return 1;
		}
		return 0;
	}

	int TogglePlayerDynamicRaceCP(int playerid, int checkpointid, bool toggle)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			boost::unordered_set<int>::iterator d = p->second.disabledRaceCheckpoints.find(checkpointid);
			if (toggle)
			{
				if (d != p->second.disabledRaceCheckpoints.end())
				{
					p->second.disabledRaceCheckpoints.quick_erase(d);
					return 1;
				}
			}
			else
			{
				if (d == p->second.disabledRaceCheckpoints.end())
				{
					if (p->second.visibleRaceCheckpoint == checkpointid)
					{
						DisablePlayerRaceCheckpoint(p->first);
						p->second.activeRaceCheckpoint = 0;
						p->second.visibleRaceCheckpoint = 0;
					}
					p->second.disabledRaceCheckpoints.insert(checkpointid);
					return 1;
				}
			}
		}
		return 0;
	}

	int TogglePlayerAllDynamicRaceCPs(int playerid, bool toggle)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			p->second.disabledRaceCheckpoints.clear();
			if (!toggle)
			{
				if (p->second.visibleRaceCheckpoint != 0)
				{
					DisablePlayerRaceCheckpoint(p->first);
					p->second.activeRaceCheckpoint = 0;
					p->second.visibleRaceCheckpoint = 0;
				}
				for (boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin(); r != core->getData()->raceCheckpoints.end(); ++r)
				{
					p->second.disabledRaceCheckpoints.insert(r->first);
				}
			}
			return 1;
		}
		return 0;
	}

	bool IsPlayerInDynamicRaceCP(int playerid, int checkpointid)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			if (p->second.activeRaceCheckpoint == checkpointid)
			{
				return 1;
			}
		}
		return 0;
	}

	int GetPlayerVisibleDynamicRaceCP(int checkpointid)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(checkpointid);
		if (p != core->getData()->players.end())
		{
			return p->second.visibleRaceCheckpoint;
		}
		return 0;
	}
};

namespace StreamerLibrary
{
	int CreateDynamicCircle(float x, float y, float size, int worldid, int interiorid, int playerid)
	{

		if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
		{
			return 0;
		}
		int areaID = Item::Area::identifier.get();
		Item::SharedArea area(new Item::Area);
		area->amx = &StreamerAMX;
		area->areaID = areaID;
		area->type = STREAMER_AREA_TYPE_CIRCLE;
		area->position = Eigen::Vector2f(x, y);
		area->size = size * size;
		Utility::addToContainer(area->worlds, worldid);
		Utility::addToContainer(area->interiors, interiorid);
		Utility::addToContainer(area->players, playerid);
		core->getGrid()->addArea(area);
		core->getData()->areas.insert(std::make_pair(areaID, area));
		return static_cast<cell>(areaID);
	}

	int CreateDynamicCylinder(float x, float y, float minz, float maxz, float size, int worldid, int interiorid, int playerid)
	{

		if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
		{
			return 0;
		}
		int areaID = Item::Area::identifier.get();
		Item::SharedArea area(new Item::Area);
		area->amx = &StreamerAMX;
		area->areaID = areaID;
		area->type = STREAMER_AREA_TYPE_CYLINDER;
		area->position = Eigen::Vector2f(x, y);
		area->height = Eigen::Vector2f(minz, maxz);
		area->size = size * size;
		Utility::addToContainer(area->worlds, worldid);
		Utility::addToContainer(area->interiors, interiorid);
		Utility::addToContainer(area->players, playerid);
		core->getGrid()->addArea(area);
		core->getData()->areas.insert(std::make_pair(areaID, area));
		return static_cast<cell>(areaID);
	}

	int CreateDynamicSphere(float x, float y, float z, float size, int worldid, int interiorid, int playerid)
	{

		if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
		{
			return 0;
		}
		int areaID = Item::Area::identifier.get();
		Item::SharedArea area(new Item::Area);
		area->amx = &StreamerAMX;
		area->areaID = areaID;
		area->type = STREAMER_AREA_TYPE_SPHERE;
		area->position = Eigen::Vector3f(x, y, z);
		area->size = size * size;
		Utility::addToContainer(area->worlds, worldid);
		Utility::addToContainer(area->interiors, interiorid);
		Utility::addToContainer(area->players, playerid);
		core->getGrid()->addArea(area);
		core->getData()->areas.insert(std::make_pair(areaID, area));
		return static_cast<cell>(areaID);
	}

	int CreateDynamicRectangle(float minx, float miny, float maxx, float maxy, int worldid, int interiorid, int playerid)
	{

		if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
		{
			return 0;
		}
		int areaID = Item::Area::identifier.get();
		Item::SharedArea area(new Item::Area);
		area->amx = &StreamerAMX;
		area->areaID = areaID;
		area->type = STREAMER_AREA_TYPE_RECTANGLE;
		area->position = Box2D(Eigen::Vector2f(minx, miny), Eigen::Vector2f(maxx, maxy));
		boost::geometry::correct(boost::get<Box2D>(area->position));
		area->size = static_cast<float>(boost::geometry::comparable_distance(boost::get<Box2D>(area->position).min_corner(), boost::get<Box2D>(area->position).max_corner()));
		Utility::addToContainer(area->worlds, worldid);
		Utility::addToContainer(area->interiors, interiorid);
		Utility::addToContainer(area->players, playerid);
		core->getGrid()->addArea(area);
		core->getData()->areas.insert(std::make_pair(areaID, area));
		return static_cast<cell>(areaID);
	}

	int CreateDynamicCuboid(float minx, float miny, float minz, float maxx, float maxy, float maxz, int worldid, int interiorid, int playerid)
	{

		if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
		{
			return 0;
		}
		int areaID = Item::Area::identifier.get();
		Item::SharedArea area(new Item::Area);
		area->amx = &StreamerAMX;
		area->areaID = areaID;
		area->type = STREAMER_AREA_TYPE_CUBOID;
		area->position = Box3D(Eigen::Vector3f(minx, miny, minz), Eigen::Vector3f(maxx, maxy, maxz));
		boost::geometry::correct(boost::get<Box3D>(area->position));
		area->size = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(boost::get<Box3D>(area->position).min_corner()[0], boost::get<Box3D>(area->position).min_corner()[1]), Eigen::Vector2f(boost::get<Box3D>(area->position).max_corner()[0], boost::get<Box3D>(area->position).max_corner()[1])));
		Utility::addToContainer(area->worlds, worldid);
		Utility::addToContainer(area->interiors, interiorid);
		Utility::addToContainer(area->players, playerid);
		core->getGrid()->addArea(area);
		core->getData()->areas.insert(std::make_pair(areaID, area));
		return static_cast<cell>(areaID);
	}
	int CreateDynamicCube(float minx, float miny, float minz, float maxx, float maxy, float maxz, int worldid, int interiorid, int playerid)
	{
		return CreateDynamicCuboid(minx, miny, minz, maxx, maxy, maxz, worldid, interiorid, playerid);
	}
	int CreateDynamicPolygon(float *points, float minz, float maxz, unsigned int max_points, int worldid, int interiorid, int playerid)
	{
		if (points)
		{
			if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
			{
				return 0;
			}
			if (max_points >= 2 && max_points % 2)
			{
				sampgdk::logprintf("*** CreateDynamicPolygon: Number of points must be divisible by two");
				return 0;
			}
			int areaID = Item::Area::identifier.get();
			Item::SharedArea area(new Item::Area);
			area->amx = &StreamerAMX;
			area->areaID = areaID;
			area->type = STREAMER_AREA_TYPE_POLYGON;

			Polygon2D &polygon = boost::get<Polygon2D>(area->position);
			std::vector<Eigen::Vector2f> _points;

			for (std::size_t i = 0; i < max_points; i += 2)
			{
				_points.push_back(Eigen::Vector2f(points[i], points[i + 1]));
			}
			boost::geometry::assign_points(polygon, _points);
			boost::geometry::correct(polygon);

			area->height = Eigen::Vector2f(minz, maxz);
			Box2D box = boost::geometry::return_envelope<Box2D>(boost::get<Polygon2D>(area->position));
			area->size = static_cast<float>(boost::geometry::comparable_distance(box.min_corner(), box.max_corner()));
			Utility::addToContainer(area->worlds, worldid);
			Utility::addToContainer(area->interiors, interiorid);
			Utility::addToContainer(area->players, playerid);
			core->getGrid()->addArea(area);
			core->getData()->areas.insert(std::make_pair(areaID, area));
			return static_cast<cell>(areaID);
		}
		return 0;
	}

	int DestroyDynamicArea(int areaid)
	{

		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
		if (a != core->getData()->areas.end())
		{
			Utility::destroyArea(a);
			return 1;
		}
		return 0;
	}

	bool IsValidDynamicArea(int areaid)
	{

		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
		if (a != core->getData()->areas.end())
		{
			return 1;
		}
		return 0;
	}

	int GetDynamicPolygonPoints(int areaid, float *points, unsigned int max_points)
	{
		if (points)
		{
			boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
			if (a != core->getData()->areas.end())
			{
				Polygon2D &polygon = boost::get<Polygon2D>(a->second->position);
				size_t i = 0;
				for (std::vector<Eigen::Vector2f>::iterator p = polygon.outer().begin(); p != polygon.outer().end(); ++p)
				{
					if ((i + 1) >= max_points)
					{
						return 0;
					}
					points[i++] = (p->data()[0]);
					points[i++] = (p->data()[1]);
				}
				return 1;
			}
		}
		return 0;
	}

	int GetDynamicPolygonNumberPoints(int areaid)
	{

		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
		if (a != core->getData()->areas.end())
		{
			return static_cast<cell>(boost::get<Polygon2D>(a->second->position).outer().size());
		}
		return 0;
	}

	int TogglePlayerDynamicArea(int playerid, int areaid, bool toggle)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			boost::unordered_set<int>::iterator d = p->second.disabledAreas.find(areaid);
			if (toggle)
			{
				if (d != p->second.disabledAreas.end())
				{
					p->second.disabledAreas.quick_erase(d);
					return 1;
				}
			}
			else
			{
				if (d == p->second.disabledAreas.end())
				{
					p->second.disabledAreas.insert(areaid);
					p->second.internalAreas.erase(areaid);
					return 1;
				}
			}
		}
		return 0;
	}

	int TogglePlayerAllDynamicAreas(int playerid, bool toggle)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			p->second.disabledAreas.clear();
			if (!toggle)
			{
				for (boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
				{
					p->second.disabledAreas.insert(a->first);
				}
				p->second.internalAreas.clear();
			}
			return 1;
		}
		return 0;
	}

	bool IsPlayerInDynamicArea(int playerid, int areaid, int recheck)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			if (!recheck)
			{
				boost::unordered_set<int>::iterator i = p->second.internalAreas.find(areaid);
				if (i != p->second.internalAreas.end())
				{
					return 1;
				}
			}
			else
			{
				boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
				return static_cast<cell>(Utility::isPointInArea(p->second.position, a->second)) != 0;
			}
		}
		return 0;
	}

	bool IsPlayerInAnyDynamicArea(int playerid, int recheck)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			if (!recheck)
			{
				if (!p->second.internalAreas.empty())
				{
					return 1;
				}
			}
			else
			{
				for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
				{
					if (Utility::isPointInArea(p->second.position, a->second))
					{
						return 1;
					}
				}
			}
		}
		return 0;
	}

	bool IsAnyPlayerInDynamicArea(int areaid, int recheck)
	{

		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			if (!recheck)
			{
				boost::unordered_set<int>::iterator i = p->second.internalAreas.find(areaid);
				if (i != p->second.internalAreas.end())
				{
					return 1;
				}
			}
			else
			{
				boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
				return static_cast<cell>(Utility::isPointInArea(p->second.position, a->second)) != 0;
			}
		}
		return 0;
	}

	bool IsAnyPlayerInAnyDynamicArea(int recheck)
	{

		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			if (!recheck)
			{
				if (!p->second.internalAreas.empty())
				{
					return 1;
				}
			}
			else
			{
				for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
				{
					if (Utility::isPointInArea(p->second.position, a->second))
					{
						return 1;
					}
				}
			}
		}
		return 0;
	}

	bool IsPointInDynamicArea(int areaid, float x, float y, float z)
	{

		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
		if (a != core->getData()->areas.end())
		{
			return Utility::isPointInArea(Eigen::Vector3f(x, y, z), a->second);
		}
		return 0;
	}

	bool IsPointInAnyDynamicArea(float x, float y, float z)
	{

		for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
		{
			if (Utility::isPointInArea(Eigen::Vector3f(x, y, z), a->second))
			{
				return 1;
			}
		}
		return 0;
	}

	int GetPlayerDynamicAreas(int playerid, int *areas, unsigned int max_areas)
	{
		if (areas)
		{
			boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
			if (p != core->getData()->players.end())
			{
				std::size_t i = 0;
				for (auto c = p->second.internalAreas.begin(); c != p->second.internalAreas.end(); ++c)
				{
					if (i == (max_areas))
					{
						return false;
					}
					areas[i++] = (*c);
				}
				return 1;
			}
		}
		return 0;
	}

	int GetPlayerNumberDynamicAreas(int playerid)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			return static_cast<cell>(p->second.internalAreas.size());
		}
		return 0;
	}

	int AttachDynamicAreaToObject(int areaid, int objectid, int type, int playerid)
	{

		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
		if (a != core->getData()->areas.end())
		{
			if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE)
			{
				sampgdk::logprintf("*** AttachDynamicAreaToObject: Only circles and spheres may be attached to objects");
				return 0;
			}
			if (objectid != INVALID_GENERIC_ID)
			{
				a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
				a->second->attach->object = boost::make_tuple(objectid, type, playerid);
				a->second->attach->player = INVALID_GENERIC_ID;
				a->second->attach->vehicle = INVALID_GENERIC_ID;
				core->getStreamer()->attachedAreas.insert(a->second);
			}
			else
			{
				if (a->second->attach)
				{
					if (a->second->attach->object.get<0>() != INVALID_GENERIC_ID)
					{
						a->second->attach.reset();
						core->getStreamer()->attachedAreas.erase(a->second);
						core->getGrid()->removeArea(a->second, true);
					}
				}
			}
			return 1;
		}
		return 0;
	}

	int AttachDynamicAreaToPlayer(int areaid, int playerid)
	{

		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
		if (a != core->getData()->areas.end())
		{
			if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE)
			{
				sampgdk::logprintf("*** AttachDynamicAreaToPlayer: Only circles and spheres may be attached to players");
				return 0;
			}
			if (playerid != INVALID_GENERIC_ID)
			{
				a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
				a->second->attach->object.get<0>() = INVALID_GENERIC_ID;
				a->second->attach->player = playerid;
				a->second->attach->vehicle = INVALID_GENERIC_ID;
				core->getStreamer()->attachedAreas.insert(a->second);
			}
			else
			{
				if (a->second->attach)
				{
					if (a->second->attach->player != INVALID_GENERIC_ID)
					{
						a->second->attach.reset();
						core->getStreamer()->attachedAreas.erase(a->second);
						core->getGrid()->removeArea(a->second, true);
					}
				}
			}
			return 1;
		}
		return 0;
	}

	int AttachDynamicAreaToVehicle(int areaid, int vehicleid)
	{

		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
		if (a != core->getData()->areas.end())
		{
			if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE)
			{
				sampgdk::logprintf("*** AttachDynamicAreaToVehicle: Only circles and spheres may be attached to vehicles");
				return 0;
			}
			if (vehicleid != INVALID_GENERIC_ID)
			{
				a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
				a->second->attach->object.get<0>() = INVALID_GENERIC_ID;
				a->second->attach->player = INVALID_GENERIC_ID;
				a->second->attach->vehicle = vehicleid;
				core->getStreamer()->attachedAreas.insert(a->second);
			}
			else
			{
				if (a->second->attach)
				{
					if (a->second->attach->vehicle != INVALID_GENERIC_ID)
					{
						a->second->attach.reset();
						core->getStreamer()->attachedAreas.erase(a->second);
						core->getGrid()->removeArea(a->second, true);
					}
				}
			}
			return 1;
		}
		return 0;
	}
};

namespace StreamerLibrary
{
	int CreateDynamicCP(float x, float y, float z, float size, int worldid, int interiorid, int playerid, float streamdistance)
	{

		if (core->getData()->getMaxItems(STREAMER_TYPE_CP) == core->getData()->checkpoints.size())
		{
			return 0;
		}
		int checkpointID = Item::Checkpoint::identifier.get();
		Item::SharedCheckpoint checkpoint(new Item::Checkpoint);
		checkpoint->amx = &StreamerAMX;
		checkpoint->checkpointID = checkpointID;
		checkpoint->position = Eigen::Vector3f(x, y, z);
		checkpoint->size = size;
		Utility::addToContainer(checkpoint->worlds, worldid);
		Utility::addToContainer(checkpoint->interiors, interiorid);
		Utility::addToContainer(checkpoint->players, playerid);
		checkpoint->streamDistance = streamdistance * streamdistance;
		core->getGrid()->addCheckpoint(checkpoint);
		core->getData()->checkpoints.insert(std::make_pair(checkpointID, checkpoint));
		return static_cast<cell>(checkpointID);
	}

	int DestroyDynamicCP(int checkpointid)
	{

		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(checkpointid);
		if (c != core->getData()->checkpoints.end())
		{
			Utility::destroyCheckpoint(c);
			return 1;
		}
		return 0;
	}

	bool IsValidDynamicCP(int checkpointid)
	{

		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(checkpointid);
		if (c != core->getData()->checkpoints.end())
		{
			return 1;
		}
		return 0;
	}

	int TogglePlayerDynamicCP(int playerid, int checkpointid, bool toggle)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			boost::unordered_set<int>::iterator d = p->second.disabledCheckpoints.find(checkpointid);
			if (toggle)
			{
				if (d != p->second.disabledCheckpoints.end())
				{
					p->second.disabledCheckpoints.quick_erase(d);
					return 1;
				}
			}
			else
			{
				if (d == p->second.disabledCheckpoints.end())
				{
					if (p->second.visibleCheckpoint == checkpointid)
					{
						DisablePlayerCheckpoint(p->first);
						p->second.activeCheckpoint = 0;
						p->second.visibleCheckpoint = 0;
					}
					p->second.disabledCheckpoints.insert(checkpointid);
					return 1;
				}
			}
		}
		return 0;
	}

	int TogglePlayerAllDynamicCPs(int playerid, bool toggle)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			p->second.disabledCheckpoints.clear();
			if (!toggle)
			{
				if (p->second.visibleCheckpoint != 0)
				{
					DisablePlayerCheckpoint(p->first);
					p->second.activeCheckpoint = 0;
					p->second.visibleCheckpoint = 0;
				}
				for (boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin(); c != core->getData()->checkpoints.end(); ++c)
				{
					p->second.disabledCheckpoints.insert(c->first);
				}
			}
			return 1;
		}
		return 0;
	}

	bool IsPlayerInDynamicCP(int playerid, int checkpointid)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			if (p->second.activeCheckpoint == checkpointid)
			{
				return 1;
			}
		}
		return 0;
	}

	int GetPlayerVisibleDynamicCP(int playerid)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			return p->second.visibleCheckpoint;
		}
		return 0;
	}
};

namespace StreamerLibrary
{
	//DATA Manipulation disabled for now, not needed
	int Streamer_GetFloatData(int type, int id, int data, float &result)
	{

		//return static_cast<cell>(Manipulation::getFloatData(amx, params));
		return 0;
	}

	int Streamer_SetFloatData(int type, int id, int data, float value)
	{

		//return static_cast<cell>(Manipulation::setFloatData(amx, params));
		return 0;
	}

	int Streamer_GetIntData(int type, int id, int data)
	{

		//return static_cast<cell>(Manipulation::getIntData(amx, params));
		return 0;
	}

	int Streamer_SetIntData(int type, int id, int data, int value)
	{

		//return static_cast<cell>(Manipulation::setIntData(amx, params));
		return 0;
	}

	int Streamer_GetArrayData(int type, int id, int data, int * dest, unsigned int maxdest)
	{

		//return static_cast<cell>(Manipulation::getArrayData(amx, params));
		return 0;
	}

	int Streamer_SetArrayData(int type, int id, int data, int * src, unsigned int maxsrc)
	{

		//return static_cast<cell>(Manipulation::setArrayData(amx, params));
		return 0;
	}

	int Streamer_IsInArrayData(int type, int id, int data, int value)
	{

		//return static_cast<cell>(Manipulation::isInArrayData(amx, params));
		return 0;
	}

	int Streamer_AppendArrayData(int type, int id, int data, int value)
	{

		//return static_cast<cell>(Manipulation::appendArrayData(amx, params));
		return 0;
	}

	int Streamer_RemoveArrayData(int type, int id, int data, int value)
	{

		//return static_cast<cell>(Manipulation::removeArrayData(amx, params));
		return 0;
	}

	int Streamer_GetUpperBound(int type)
	{

		/*switch (static_cast<int>(params[1]))
		{
		case STREAMER_TYPE_OBJECT:
		{
		int objectID = 0;
		for (boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.begin(); o != core->getData()->objects.end(); ++o)
		{
		if (o->first > objectID)
		{
		objectID = o->first;
		}
		}
		//return static_cast<cell>(objectID + 1);
		}
		case STREAMER_TYPE_PICKUP:
		{
		int pickupID = 0;
		for (boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.begin(); p != core->getData()->pickups.end(); ++p)
		{
		if (p->first > pickupID)
		{
		pickupID = p->first;
		}
		}
		//return static_cast<cell>(pickupID + 1);
		}
		case STREAMER_TYPE_CP:
		{
		int checkpointID = 0;
		for (boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin(); c != core->getData()->checkpoints.end(); ++c)
		{
		if (c->first > checkpointID)
		{
		checkpointID = c->first;
		}
		}
		//return static_cast<cell>(checkpointID + 1);
		}
		case STREAMER_TYPE_RACE_CP:
		{
		int raceCheckpointID = 0;
		for (boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin(); r != core->getData()->raceCheckpoints.end(); ++r)
		{
		if (r->first > raceCheckpointID)
		{
		raceCheckpointID = r->first;
		}
		}
		//return static_cast<cell>(raceCheckpointID + 1);
		}
		case STREAMER_TYPE_MAP_ICON:
		{
		int mapIconID = 0;
		for (boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.begin(); m != core->getData()->mapIcons.end(); ++m)
		{
		if (m->first > mapIconID)
		{
		mapIconID = m->first;
		}
		}
		//return static_cast<cell>(mapIconID + 1);
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
		int textLabelID = 0;
		for (boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.begin(); t != core->getData()->textLabels.end(); ++t)
		{
		if (t->first > textLabelID)
		{
		textLabelID = t->first;
		}
		}
		//return static_cast<cell>(textLabelID + 1);
		}
		case STREAMER_TYPE_AREA:
		{
		int areaID = 0;
		for (boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
		{
		if (a->first > areaID)
		{
		areaID = a->first;
		}
		}
		//return static_cast<cell>(areaID + 1);
		}
		default:
		{
		sampgdk::logprintf("*** Streamer_GetUpperBound: Invalid type specified");
		return 0;
		}
		}*/
		return 0;
	}
};

namespace StreamerLibrary
{
	int Streamer_CallbackHook(int callback, ...)
	{
		return 0;
	}
	//notimplemented too

	int DestroyAllDynamicObjects()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_OBJECT, 1 };
		//return Streamer_DestroyAllItems(amx, newParams);
		return 0;
	}

	int CountDynamicObjects()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_OBJECT, 1 };
		//return Streamer_CountItems(amx, newParams);
		return 0;
	}

	int DestroyAllDynamicPickups()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_PICKUP, 1 };
		//return Streamer_DestroyAllItems(amx, newParams);
		return 0;
	}

	int CountDynamicPickups()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_PICKUP, 1 };
		//return Streamer_CountItems(amx, newParams);
		return 0;
	}

	int DestroyAllDynamicCPs()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_CP, 1 };
		//return Streamer_DestroyAllItems(amx, newParams);
		return 0;
	}

	int CountDynamicCPs()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_CP, 1 };
		//return Streamer_CountItems(amx, newParams);
		return 0;
	}

	int DestroyAllDynamicRaceCPs()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_RACE_CP, 1 };
		//return Streamer_DestroyAllItems(amx, newParams);
		return 0;
	}

	int CountDynamicRaceCPs()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_RACE_CP, 1 };
		//return Streamer_CountItems(amx, newParams);
		return 0;
	}

	int DestroyAllDynamicMapIcons()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_MAP_ICON, 1 };
		//return Streamer_DestroyAllItems(amx, newParams);
		return 0;
	}

	int CountDynamicMapIcons()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_MAP_ICON, 1 };
		//return Streamer_CountItems(amx, newParams);
		return 0;
	}

	int DestroyAllDynamic3DTextLabels()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_3D_TEXT_LABEL, 1 };
		//return Streamer_DestroyAllItems(amx, newParams);
		return 0;
	}

	int CountDynamic3DTextLabels()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_3D_TEXT_LABEL, 1 };
		//return Streamer_CountItems(amx, newParams);
		return 0;
	}

	int DestroyAllDynamicAreas()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_AREA, 1 };
		//return Streamer_DestroyAllItems(amx, newParams);
		return 0;
	}

	int CountDynamicAreas()
	{
		//cell newParams[3] = { sizeof(cell) * 2, STREAMER_TYPE_AREA, 1 };
		//return Streamer_CountItems(amx, newParams);
		return 0;
	}
};

namespace StreamerLibrary
{
	//not implemented not needed
	int CreateDynamicObjectEx(int modelid, float x, float y, float z, float rx, float ry, float rz, float drawdistance, float streamdistance, int *worlds, int *interiors, int *players, unsigned int max_worlds, unsigned int max_interiors, unsigned int max_players)
	{

		/*if (core->getData()->getMaxItems(STREAMER_TYPE_OBJECT) == core->getData()->objects.size())
		{
		return 0;
		}
		int objectID = Item::Object::identifier.get();
		Item::SharedObject object(new Item::Object);
		object->amx = amx;
		object->objectID = objectID;
		object->modelID = static_cast<int>(params[1]);
		object->position = Eigen::Vector3f(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));
		object->rotation = Eigen::Vector3f(amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]));
		object->drawDistance = amx_ctof(params[8]);
		object->streamDistance = amx_ctof(params[9]) * amx_ctof(params[9]);
		Utility::convertArrayToContainer(amx, params[10], params[13], object->worlds);
		Utility::convertArrayToContainer(amx, params[11], params[14], object->interiors);
		Utility::convertArrayToContainer(amx, params[12], params[15], object->players);
		core->getGrid()->addObject(object);
		core->getData()->objects.insert(std::make_pair(objectID, object));
		return static_cast<cell>(objectID);*/
		return 0;
	}

	int CreateDynamicPickupEx(int modelid, int type, float x, float y, float z, float streamdistance, int *worlds, int *interiors, int *players, unsigned int max_worlds, unsigned int max_interiors, unsigned int max_players)
	{

		/*if (core->getData()->getMaxItems(STREAMER_TYPE_PICKUP) == core->getData()->pickups.size())
		{
		return 0;
		}
		int pickupID = Item::Pickup::identifier.get();
		Item::SharedPickup pickup(new Item::Pickup);
		pickup->amx = amx;
		pickup->pickupID = pickupID;
		pickup->modelID = static_cast<int>(params[1]);
		pickup->type = static_cast<int>(params[2]);
		pickup->position = Eigen::Vector3f(amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]));
		pickup->streamDistance = amx_ctof(params[6]) * amx_ctof(params[6]);
		Utility::convertArrayToContainer(amx, params[7], params[10], pickup->worlds);
		Utility::convertArrayToContainer(amx, params[8], params[11], pickup->interiors);
		Utility::convertArrayToContainer(amx, params[9], params[12], pickup->players);
		core->getGrid()->addPickup(pickup);
		core->getData()->pickups.insert(std::make_pair(pickupID, pickup));
		return static_cast<cell>(pickupID);*/
		return 0;
	}

	int CreateDynamicCPEx(float x, float y, float z, float size, float streamdistance, int *worlds, int *interiors, int *players, unsigned int max_worlds, unsigned int max_interiors, unsigned int max_players)
	{

		/*if (core->getData()->getMaxItems(STREAMER_TYPE_CP) == core->getData()->checkpoints.size())
		{
		return 0;
		}
		int checkpointID = Item::Checkpoint::identifier.get();
		Item::SharedCheckpoint checkpoint(new Item::Checkpoint);
		checkpoint->amx = amx;
		checkpoint->checkpointID = checkpointID;
		checkpoint->position = Eigen::Vector3f(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]));
		checkpoint->size = amx_ctof(params[4]);
		checkpoint->streamDistance = amx_ctof(params[5]) * amx_ctof(params[5]);
		Utility::convertArrayToContainer(amx, params[6], params[9], checkpoint->worlds);
		Utility::convertArrayToContainer(amx, params[7], params[10], checkpoint->interiors);
		Utility::convertArrayToContainer(amx, params[8], params[11], checkpoint->players);
		core->getGrid()->addCheckpoint(checkpoint);
		core->getData()->checkpoints.insert(std::make_pair(checkpointID, checkpoint));
		return static_cast<cell>(checkpointID);*/
		return 0;
	}

	int CreateDynamicRaceCPEx(int type, float x, float y, float z, float nextx, float nexty, float nextz, float size, float streamdistance, int *worlds, int *interiors, int *players, unsigned int max_worlds, unsigned int max_interiors, unsigned int max_players)
	{

		/*if (core->getData()->getMaxItems(STREAMER_TYPE_RACE_CP) == core->getData()->raceCheckpoints.size())
		{
		return 0;
		}
		int raceCheckpointID = Item::RaceCheckpoint::identifier.get();
		Item::SharedRaceCheckpoint raceCheckpoint(new Item::RaceCheckpoint);
		raceCheckpoint->amx = amx;
		raceCheckpoint->raceCheckpointID = raceCheckpointID;
		raceCheckpoint->type = static_cast<int>(params[1]);
		raceCheckpoint->position = Eigen::Vector3f(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));
		raceCheckpoint->next = Eigen::Vector3f(amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]));
		raceCheckpoint->size = amx_ctof(params[8]);
		raceCheckpoint->streamDistance = amx_ctof(params[9]) * amx_ctof(params[9]);
		Utility::convertArrayToContainer(amx, params[10], params[13], raceCheckpoint->worlds);
		Utility::convertArrayToContainer(amx, params[11], params[14], raceCheckpoint->interiors);
		Utility::convertArrayToContainer(amx, params[12], params[15], raceCheckpoint->players);
		core->getGrid()->addRaceCheckpoint(raceCheckpoint);
		core->getData()->raceCheckpoints.insert(std::make_pair(raceCheckpointID, raceCheckpoint));
		return static_cast<cell>(raceCheckpointID);*/
		return 0;
	}

	int CreateDynamicMapIconEx(float x, float y, float z, int type, int color, int style, float streamdistance, int *worlds, int *interiors, int *players, unsigned int max_worlds, unsigned int max_interiors, unsigned int max_players)
	{

		/*if (core->getData()->getMaxItems(STREAMER_TYPE_MAP_ICON) == core->getData()->mapIcons.size())
		{
		return 0;
		}
		int mapIconID = Item::MapIcon::identifier.get();
		Item::SharedMapIcon mapIcon(new Item::MapIcon);
		mapIcon->amx = amx;
		mapIcon->mapIconID = mapIconID;
		mapIcon->position = Eigen::Vector3f(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]));
		mapIcon->type = static_cast<int>(params[4]);
		mapIcon->color = static_cast<int>(params[5]);
		mapIcon->style = static_cast<int>(params[6]);
		mapIcon->streamDistance = amx_ctof(params[7]) * amx_ctof(params[7]);
		Utility::convertArrayToContainer(amx, params[8], params[11], mapIcon->worlds);
		Utility::convertArrayToContainer(amx, params[9], params[12], mapIcon->interiors);
		Utility::convertArrayToContainer(amx, params[10], params[13], mapIcon->players);
		core->getGrid()->addMapIcon(mapIcon);
		core->getData()->mapIcons.insert(std::make_pair(mapIconID, mapIcon));
		return static_cast<cell>(mapIconID);*/
		return 0;
	}

	int CreateDynamic3DTextLabelEx(std::string text, int color, float x, float y, float z, float drawdistance, int attachedplayer, int attachedvehicle, bool testlos, float streamdistance, int *worlds, int *interiors, int *players, unsigned int max_worlds, unsigned int max_interiors, unsigned int max_players)
	{

		/*if (core->getData()->getMaxItems(STREAMER_TYPE_3D_TEXT_LABEL) == core->getData()->textLabels.size())
		{
		return 0;
		}
		int textLabelID = Item::TextLabel::identifier.get();
		Item::SharedTextLabel textLabel(new Item::TextLabel);
		textLabel->amx = amx;
		textLabel->textLabelID = textLabelID;
		textLabel->text = Utility::convertNativeStringToString(amx, params[1]);
		textLabel->color = static_cast<int>(params[2]);
		textLabel->position = Eigen::Vector3f(amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]));
		textLabel->drawDistance = amx_ctof(params[6]);
		if (static_cast<int>(params[7]) != INVALID_GENERIC_ID || static_cast<int>(params[8]) != INVALID_GENERIC_ID)
		{
		textLabel->attach = boost::intrusive_ptr<Item::TextLabel::Attach>(new Item::TextLabel::Attach);
		textLabel->attach->player = static_cast<int>(params[7]);
		textLabel->attach->vehicle = static_cast<int>(params[8]);
		if (textLabel->position.cwiseAbs().maxCoeff() > 50.0f)
		{
		textLabel->position.setZero();
		}
		core->getStreamer()->attachedTextLabels.insert(textLabel);
		}
		textLabel->testLOS = static_cast<int>(params[9]) != 0;
		textLabel->streamDistance = amx_ctof(params[10]) * amx_ctof(params[10]);
		Utility::convertArrayToContainer(amx, params[11], params[14], textLabel->worlds);
		Utility::convertArrayToContainer(amx, params[12], params[15], textLabel->interiors);
		Utility::convertArrayToContainer(amx, params[13], params[16], textLabel->players);
		core->getGrid()->addTextLabel(textLabel);
		core->getData()->textLabels.insert(std::make_pair(textLabelID, textLabel));
		return static_cast<cell>(textLabelID);*/
		return 0;
	}

	int CreateDynamicCircleEx(float x, float y, float size, int *worlds, int *interiors, int *players, unsigned int max_worlds, unsigned int max_interiors, unsigned int max_players)
	{

		/*if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
		{
		return 0;
		}
		int areaID = Item::Area::identifier.get();
		Item::SharedArea area(new Item::Area);
		area->amx = amx;
		area->areaID = areaID;
		area->type = STREAMER_AREA_TYPE_CIRCLE;
		area->position = Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2]));
		area->size = amx_ctof(params[3]) * amx_ctof(params[3]);
		Utility::convertArrayToContainer(amx, params[4], params[7], area->worlds);
		Utility::convertArrayToContainer(amx, params[5], params[8], area->interiors);
		Utility::convertArrayToContainer(amx, params[6], params[9], area->players);
		core->getGrid()->addArea(area);
		core->getData()->areas.insert(std::make_pair(areaID, area));
		return static_cast<cell>(areaID);*/
		return 0;
	}

	int CreateDynamicCylinderEx(float x, float y, float minz, float maxz, float size, int *worlds, int *interiors, int *players, unsigned int max_worlds, unsigned int max_interiors, unsigned int max_players)
	{

		/*if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
		{
		return 0;
		}
		int areaID = Item::Area::identifier.get();
		Item::SharedArea area(new Item::Area);
		area->amx = amx;
		area->areaID = areaID;
		area->type = STREAMER_AREA_TYPE_CYLINDER;
		area->position = Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2]));
		area->height = Eigen::Vector2f(amx_ctof(params[3]), amx_ctof(params[4]));
		area->size = amx_ctof(params[5]) * amx_ctof(params[5]);
		Utility::convertArrayToContainer(amx, params[6], params[9], area->worlds);
		Utility::convertArrayToContainer(amx, params[7], params[10], area->interiors);
		Utility::convertArrayToContainer(amx, params[8], params[11], area->players);
		core->getGrid()->addArea(area);
		core->getData()->areas.insert(std::make_pair(areaID, area));
		return static_cast<cell>(areaID);*/
		return 0;
	}

	int CreateDynamicSphereEx(float x, float y, float z, float size, int *worlds, int *interiors, int *players, unsigned int max_worlds, unsigned int max_interiors, unsigned int max_players)
	{

		/*if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
		{
		return 0;
		}
		int areaID = Item::Area::identifier.get();
		Item::SharedArea area(new Item::Area);
		area->amx = amx;
		area->areaID = areaID;
		area->type = STREAMER_AREA_TYPE_SPHERE;
		area->position = Eigen::Vector3f(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]));
		area->size = amx_ctof(params[4]) * amx_ctof(params[4]);
		Utility::convertArrayToContainer(amx, params[5], params[8], area->worlds);
		Utility::convertArrayToContainer(amx, params[6], params[9], area->interiors);
		Utility::convertArrayToContainer(amx, params[7], params[10], area->players);
		core->getGrid()->addArea(area);
		core->getData()->areas.insert(std::make_pair(areaID, area));
		return static_cast<cell>(areaID);*/
		return 0;
	}

	int CreateDynamicRectangleEx(float minx, float miny, float maxx, float maxy, int *worlds, int *interiors, int *players, unsigned int max_worlds, unsigned int max_interiors, unsigned int max_players)
	{

		/*if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
		{
		return 0;
		}
		int areaID = Item::Area::identifier.get();
		Item::SharedArea area(new Item::Area);
		area->amx = amx;
		area->areaID = areaID;
		area->type = STREAMER_AREA_TYPE_RECTANGLE;
		area->position = Box2D(Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2])), Eigen::Vector2f(amx_ctof(params[3]), amx_ctof(params[4])));
		boost::geometry::correct(boost::get<Box2D>(area->position));
		area->size = static_cast<float>(boost::geometry::comparable_distance(boost::get<Box2D>(area->position).min_corner(), boost::get<Box2D>(area->position).max_corner()));
		Utility::convertArrayToContainer(amx, params[5], params[8], area->worlds);
		Utility::convertArrayToContainer(amx, params[6], params[9], area->interiors);
		Utility::convertArrayToContainer(amx, params[7], params[10], area->players);
		core->getGrid()->addArea(area);
		core->getData()->areas.insert(std::make_pair(areaID, area));
		return static_cast<cell>(areaID);*/
		return 0;
	}

	int CreateDynamicCuboidEx(float minx, float miny, float minz, float maxx, float maxy, float maxz, int *worlds, int *interiors, int *players, unsigned int max_worlds, unsigned int max_interiors, unsigned int max_players)
	{

		/*if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
		{
		return 0;
		}
		int areaID = Item::Area::identifier.get();
		Item::SharedArea area(new Item::Area);
		area->amx = amx;
		area->areaID = areaID;
		area->type = STREAMER_AREA_TYPE_CUBOID;
		area->position = Box3D(Eigen::Vector3f(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3])), Eigen::Vector3f(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6])));
		boost::geometry::correct(boost::get<Box3D>(area->position));
		area->size = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(boost::get<Box3D>(area->position).min_corner()[0], boost::get<Box3D>(area->position).min_corner()[1]), Eigen::Vector2f(boost::get<Box3D>(area->position).max_corner()[0], boost::get<Box3D>(area->position).max_corner()[1])));
		Utility::convertArrayToContainer(amx, params[7], params[10], area->worlds);
		Utility::convertArrayToContainer(amx, params[8], params[11], area->interiors);
		Utility::convertArrayToContainer(amx, params[9], params[12], area->players);
		core->getGrid()->addArea(area);
		core->getData()->areas.insert(std::make_pair(areaID, area));
		return static_cast<cell>(areaID);*/
		return 0;
	}

	int CreateDynamicPolygonEx(float *point, float minz, float maxz, unsigned int max_points, int *worlds, int *interiors, int *players, unsigned int max_worlds, unsigned int max_interiors, unsigned int max_players)
	{

		/*if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
		{
		return 0;
		}
		if (static_cast<int>(params[4] >= 2 && static_cast<int>(params[4]) % 2))
		{
		sampgdk::logprintf("*** CreateDynamicPolygonEx: Number of points must be divisible by two");
		}
		int areaID = Item::Area::identifier.get();
		Item::SharedArea area(new Item::Area);
		area->amx = amx;
		area->areaID = areaID;
		area->type = STREAMER_AREA_TYPE_POLYGON;
		Utility::convertArrayToPolygon(amx, params[1], params[4], boost::get<Polygon2D>(area->position));
		area->height = Eigen::Vector2f(amx_ctof(params[2]), amx_ctof(params[3]));
		Box2D box = boost::geometry::return_envelope<Box2D>(boost::get<Polygon2D>(area->position));
		area->size = static_cast<float>(boost::geometry::comparable_distance(box.min_corner(), box.max_corner()));
		Utility::convertArrayToContainer(amx, params[5], params[8], area->worlds);
		Utility::convertArrayToContainer(amx, params[6], params[9], area->interiors);
		Utility::convertArrayToContainer(amx, params[7], params[10], area->players);
		core->getGrid()->addArea(area);
		core->getData()->areas.insert(std::make_pair(areaID, area));
		return static_cast<cell>(areaID);*/
		return 0;
	}
};

namespace StreamerLibrary
{
	int CreateDynamicMapIcon(float x, float y, float z, int type, int color, int worldid, int interiorid, int playerid, float streamdistance, int style)
	{

		if (core->getData()->getMaxItems(STREAMER_TYPE_MAP_ICON) == core->getData()->mapIcons.size())
		{
			return 0;
		}
		int mapIconID = Item::MapIcon::identifier.get();
		Item::SharedMapIcon mapIcon(new Item::MapIcon);
		mapIcon->amx = &StreamerAMX;
		mapIcon->mapIconID = mapIconID;
		mapIcon->position = Eigen::Vector3f(x, y, z);
		mapIcon->type = type;
		mapIcon->color = color;
		Utility::addToContainer(mapIcon->worlds, worldid);
		Utility::addToContainer(mapIcon->interiors, interiorid);
		Utility::addToContainer(mapIcon->players, playerid);;
		mapIcon->streamDistance = streamdistance * streamdistance;
		mapIcon->style = style;
		core->getGrid()->addMapIcon(mapIcon);
		core->getData()->mapIcons.insert(std::make_pair(mapIconID, mapIcon));
		return static_cast<cell>(mapIconID);
	}

	int DestroyDynamicMapIcon(int iconid)
	{

		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(iconid);
		if (m != core->getData()->mapIcons.end())
		{
			Utility::destroyMapIcon(m);
			return 1;
		}
		return 0;
	}

	int IsValidDynamicMapIcon(int iconid)
	{

		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(iconid);
		if (m != core->getData()->mapIcons.end())
		{
			return 1;
		}
		return 0;
	}
};

namespace StreamerLibrary
{
	//until needed not implemented
	int Streamer_GetDistanceToItem(float x, float y, float z, int type, int id, float &distance, int dimensions)
	{

		/*int dimensions = static_cast<int>(params[7]);
		Eigen::Vector3f position = Eigen::Vector3f::Zero();
		switch (static_cast<int>(params[4]))
		{
		case STREAMER_TYPE_OBJECT:
		{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(static_cast<int>(params[5]));
		if (o != core->getData()->objects.end())
		{
		position = o->second->position;
		break;
		}
		return 0;
		}
		case STREAMER_TYPE_PICKUP:
		{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(static_cast<int>(params[5]));
		if (p != core->getData()->pickups.end())
		{
		position = p->second->position;
		break;
		}
		return 0;
		}
		case STREAMER_TYPE_CP:
		{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(static_cast<int>(params[5]));
		if (c != core->getData()->checkpoints.end())
		{
		position = c->second->position;
		break;
		}
		return 0;
		}
		case STREAMER_TYPE_RACE_CP:
		{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(static_cast<int>(params[5]));
		if (r != core->getData()->raceCheckpoints.end())
		{
		position = r->second->position;
		break;
		}
		return 0;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(static_cast<int>(params[5]));
		if (m != core->getData()->mapIcons.end())
		{
		position = m->second->position;
		break;
		}
		return 0;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(static_cast<int>(params[5]));
		if (t != core->getData()->textLabels.end())
		{
		if (t->second->attach)
		{
		position = t->second->attach->position;
		}
		else
		{
		position = t->second->position;
		}
		break;
		}
		return 0;
		}
		case STREAMER_TYPE_AREA:
		{
		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[5]));
		if (a != core->getData()->areas.end())
		{
		switch (a->second->type)
		{
		case STREAMER_AREA_TYPE_CIRCLE:
		case STREAMER_AREA_TYPE_CYLINDER:
		{
		float distance = 0.0f;
		if (a->second->attach)
		{
		distance = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2])), Eigen::Vector2f(a->second->attach->position[0], a->second->attach->position[1])));
		}
		else
		{
		distance = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2])), boost::get<Eigen::Vector2f>(a->second->position)));
		}
		Utility::storeFloatInNative(amx, params[6], distance);
		return 1;
		}
		case STREAMER_AREA_TYPE_SPHERE:
		{
		if (a->second->attach)
		{
		position = a->second->attach->position;
		}
		else
		{
		position = boost::get<Eigen::Vector3f>(a->second->position);
		}
		break;
		}
		case STREAMER_AREA_TYPE_RECTANGLE:
		{
		Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(boost::get<Box2D>(a->second->position));
		float distance = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2])), centroid));
		Utility::storeFloatInNative(amx, params[6], distance);
		return 1;
		}
		case STREAMER_AREA_TYPE_CUBOID:
		{
		Eigen::Vector3f centroid = boost::geometry::return_centroid<Eigen::Vector3f>(boost::get<Box3D>(a->second->position));
		float distance = static_cast<float>(boost::geometry::distance(Eigen::Vector3f(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3])), centroid));
		Utility::storeFloatInNative(amx, params[6], distance);
		return 1;
		}
		case STREAMER_AREA_TYPE_POLYGON:
		{
		Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(boost::get<Polygon2D>(a->second->position));
		float distance = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2])), centroid));
		Utility::storeFloatInNative(amx, params[6], distance);
		return 1;
		}
		}
		}
		return 0;
		}
		default:
		{
		sampgdk::logprintf("*** Streamer_GetDistanceToItem: Invalid type specified");
		return 0;
		}
		}
		switch (dimensions)
		{
		case 2:
		{
		float distance = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2])), Eigen::Vector2f(position[0], position[1])));
		Utility::storeFloatInNative(amx, params[6], distance);
		return 1;
		}
		case 3:
		{
		float distance = static_cast<float>(boost::geometry::distance(Eigen::Vector3f(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3])), position));
		Utility::storeFloatInNative(amx, params[6], distance);
		return 1;
		}
		default:
		{
		sampgdk::logprintf("*** Streamer_GetDistanceToItem: Invalid number of dimensions specified (outside range of 2-3)");
		return 0;
		}
		}*/
		return 0;
	}

	int Streamer_GetItemInternalID(int playerid, int type, int streamerid)
	{

		/*if (static_cast<int>(params[2]) == STREAMER_TYPE_PICKUP)
		{
		boost::unordered_map<int, int>::iterator i = core->getStreamer()->internalPickups.find(static_cast<int>(params[3]));
		if (i != core->getStreamer()->internalPickups.end())
		{
		return static_cast<cell>(i->second);
		}
		return 0;
		}
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
		if (p != core->getData()->players.end())
		{
		switch (static_cast<int>(params[2]))
		{
		case STREAMER_TYPE_OBJECT:
		{
		boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(static_cast<int>(params[3]));
		if (i != p->second.internalObjects.end())
		{
		return static_cast<cell>(i->second);
		}
		return 0;
		}
		case STREAMER_TYPE_CP:
		{
		if (p->second.visibleCheckpoint == static_cast<int>(params[3]))
		{
		return 1;
		}
		return 0;
		}
		case STREAMER_TYPE_RACE_CP:
		{
		if (p->second.visibleRaceCheckpoint == static_cast<int>(params[3]))
		{
		return 1;
		}
		return 0;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
		boost::unordered_map<int, int>::iterator i = p->second.internalMapIcons.find(static_cast<int>(params[3]));
		if (i != p->second.internalMapIcons.end())
		{
		return static_cast<cell>(i->second);
		}
		return 0;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
		boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(static_cast<int>(params[3]));
		if (i != p->second.internalTextLabels.end())
		{
		return static_cast<cell>(i->second);
		}
		}
		case STREAMER_TYPE_AREA:
		{
		boost::unordered_set<int>::iterator i = p->second.internalAreas.find(static_cast<int>(params[3]));
		if (i != p->second.internalAreas.end())
		{
		return *i;
		}
		return 0;
		}
		default:
		{
		sampgdk::logprintf("*** Streamer_GetItemInternalID: Invalid type specified");
		return 0;
		}
		}
		}*/
		return 0;
	}

	int Streamer_GetItemStreamerID(int playerid, int type, int internalid)
	{

		/*if (static_cast<int>(params[2]) == STREAMER_TYPE_PICKUP)
		{
		for (boost::unordered_map<int, int>::iterator i = core->getStreamer()->internalPickups.begin(); i != core->getStreamer()->internalPickups.end(); ++i)
		{
		if (i->second == static_cast<int>(params[3]))
		{
		return i->first;
		}
		}
		return 0;
		}
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
		if (p != core->getData()->players.end())
		{
		switch (static_cast<int>(params[2]))
		{
		case STREAMER_TYPE_OBJECT:
		{
		for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i)
		{
		if (i->second == static_cast<int>(params[3]))
		{
		return i->first;
		}
		}
		return 0;
		}
		case STREAMER_TYPE_CP:
		{
		if (p->second.visibleCheckpoint == static_cast<int>(params[3]))
		{
		return 1;
		}
		return 0;
		}
		case STREAMER_TYPE_RACE_CP:
		{
		if (p->second.visibleRaceCheckpoint == static_cast<int>(params[3]))
		{
		return 1;
		}
		return 0;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
		for (boost::unordered_map<int, int>::iterator i = p->second.internalMapIcons.begin(); i != p->second.internalMapIcons.end(); ++i)
		{
		if (i->second == static_cast<int>(params[3]))
		{
		return i->first;
		}
		}
		return 0;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
		for (boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.begin(); i != p->second.internalTextLabels.end(); ++i)
		{
		if (i->second == static_cast<int>(params[3]))
		{
		return i->first;
		}
		}
		return 0;
		}
		case STREAMER_TYPE_AREA:
		{
		boost::unordered_set<int>::iterator i = p->second.internalAreas.find(static_cast<int>(params[3]));
		if (i != p->second.internalAreas.end())
		{
		return *i;
		}
		return 0;
		}
		default:
		{
		sampgdk::logprintf("*** Streamer_GetItemStreamerID: Invalid type specified");
		return 0;
		}
		}
		}*/
		return 0;
	}

	int Streamer_IsItemVisible(int playerid, int type, int id)
	{

		/*if (static_cast<int>(params[2]) == STREAMER_TYPE_PICKUP)
		{
		boost::unordered_map<int, int>::iterator i = core->getStreamer()->internalPickups.find(static_cast<int>(params[3]));
		if (i != core->getStreamer()->internalPickups.end())
		{
		return 1;
		}
		return 0;
		}
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
		if (p != core->getData()->players.end())
		{
		switch (static_cast<int>(params[2]))
		{
		case STREAMER_TYPE_OBJECT:
		{
		boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(static_cast<int>(params[3]));
		if (i != p->second.internalObjects.end())
		{
		return 1;
		}
		return 0;
		}
		case STREAMER_TYPE_CP:
		{
		if (p->second.visibleCheckpoint == static_cast<int>(params[3]))
		{
		return 1;
		}
		return 0;
		}
		case STREAMER_TYPE_RACE_CP:
		{
		if (p->second.visibleRaceCheckpoint == static_cast<int>(params[3]))
		{
		return 1;
		}
		return 0;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
		boost::unordered_map<int, int>::iterator i = p->second.internalMapIcons.find(static_cast<int>(params[3]));
		if (i != p->second.internalMapIcons.end())
		{
		return 1;
		}
		return 0;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
		boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(static_cast<int>(params[3]));
		if (i != p->second.internalTextLabels.end())
		{
		return 1;
		}
		}
		case STREAMER_TYPE_AREA:
		{
		boost::unordered_set<int>::iterator i = p->second.internalAreas.find(static_cast<int>(params[3]));
		if (i != p->second.internalAreas.end())
		{
		return 1;
		}
		return 0;
		}
		default:
		{
		sampgdk::logprintf("*** Streamer_IsItemVisible: Invalid type specified");
		return 0;
		}
		}
		}*/
		return 0;
	}

	int Streamer_DestroyAllVisibleItems(int playerid, int type, int serverwide)
	{

		/*bool serverWide = static_cast<int>(params[3]) != 0;
		if (static_cast<int>(params[2]) == STREAMER_TYPE_PICKUP)
		{
		boost::unordered_map<int, int>::iterator p = core->getStreamer()->internalPickups.begin();
		while (p != core->getStreamer()->internalPickups.end())
		{
		boost::unordered_map<int, Item::SharedPickup>::iterator q = core->getData()->pickups.find(p->first);
		if (serverWide || (q != core->getData()->pickups.end() && q->second->amx == amx))
		{
		DestroyPickup(p->second);
		p = core->getStreamer()->internalPickups.erase(p);
		}
		else
		{
		++p;
		}
		}
		return 1;
		}
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
		if (p != core->getData()->players.end())
		{
		switch (static_cast<int>(params[2]))
		{
		case STREAMER_TYPE_OBJECT:
		{
		boost::unordered_map<int, int>::iterator o = p->second.internalObjects.begin();
		while (o != p->second.internalObjects.end())
		{
		boost::unordered_map<int, Item::SharedObject>::iterator q = core->getData()->objects.find(o->first);
		if (serverWide || (q != core->getData()->objects.end() && q->second->amx == amx))
		{
		DestroyPlayerObject(p->first, o->second);
		o = p->second.internalObjects.erase(o);
		}
		else
		{
		++o;
		}
		}
		return 1;
		}
		case STREAMER_TYPE_CP:
		{
		if (p->second.visibleCheckpoint)
		{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(p->second.visibleCheckpoint);
		if (serverWide || (c != core->getData()->checkpoints.end() && c->second->amx == amx))
		{
		DisablePlayerCheckpoint(p->first);
		p->second.activeCheckpoint = 0;
		p->second.visibleCheckpoint = 0;
		return 1;
		}
		}
		return 0;
		}
		case STREAMER_TYPE_RACE_CP:
		{
		if (p->second.visibleRaceCheckpoint)
		{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(p->second.visibleRaceCheckpoint);
		if (serverWide || (r != core->getData()->raceCheckpoints.end() && r->second->amx == amx))
		{
		DisablePlayerRaceCheckpoint(p->first);
		p->second.activeRaceCheckpoint = 0;
		p->second.visibleRaceCheckpoint = 0;
		return 1;
		}
		}
		return 0;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
		boost::unordered_map<int, int>::iterator m = p->second.internalMapIcons.begin();
		while (m != p->second.internalMapIcons.end())
		{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator n = core->getData()->mapIcons.find(m->first);
		if (serverWide || (n != core->getData()->mapIcons.end() && n->second->amx == amx))
		{
		RemovePlayerMapIcon(p->first, m->second);
		m = p->second.internalMapIcons.erase(m);
		}
		else
		{
		++m;
		}
		}
		return 1;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
		boost::unordered_map<int, int>::iterator t = p->second.internalTextLabels.begin();
		while (t != p->second.internalMapIcons.end())
		{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator u = core->getData()->textLabels.find(t->first);
		if (serverWide || (u != core->getData()->textLabels.end() && u->second->amx == amx))
		{
		DeletePlayer3DTextLabel(p->first, t->second);
		t = p->second.internalTextLabels.erase(t);
		}
		else
		{
		++t;
		}
		}
		return 1;
		}
		case STREAMER_TYPE_AREA:
		{
		boost::unordered_set<int>::iterator a = p->second.internalAreas.begin();
		while (a != p->second.internalAreas.end())
		{
		boost::unordered_map<int, Item::SharedArea>::iterator b = core->getData()->areas.find(*a);
		if (serverWide || (b != core->getData()->areas.end() && b->second->amx == amx))
		{
		a = p->second.internalAreas.erase(a);
		}
		else
		{
		++a;
		}
		}
		return 1;
		}
		default:
		{
		sampgdk::logprintf("*** Streamer_DestroyAllVisibleItems: Invalid type specified");
		return 0;
		}
		}
		}*/
		return 0;
	}

	int Streamer_CountVisibleItems(int playerid, int type, int serverwide)
	{

		/*bool serverWide = static_cast<int>(params[3]) != 0;
		if (static_cast<int>(params[2]) == STREAMER_TYPE_PICKUP)
		{
		return static_cast<cell>(core->getStreamer()->internalPickups.size());
		}
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
		if (p != core->getData()->players.end())
		{
		switch (static_cast<int>(params[2]))
		{
		case STREAMER_TYPE_OBJECT:
		{
		if (serverWide)
		{
		return static_cast<cell>(p->second.internalObjects.size());
		}
		else
		{
		int count = 0;
		for (boost::unordered_map<int, int>::iterator o = p->second.internalObjects.begin(); o != p->second.internalObjects.end(); ++o)
		{
		boost::unordered_map<int, Item::SharedObject>::iterator q = core->getData()->objects.find(o->first);
		if (q != core->getData()->objects.end() && q->second->amx == amx)
		{
		++count;
		}
		}
		return static_cast<cell>(count);
		}
		}
		case STREAMER_TYPE_CP:
		{
		if (p->second.visibleCheckpoint)
		{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(p->second.visibleCheckpoint);
		if (serverWide || (c != core->getData()->checkpoints.end() && c->second->amx == amx))
		{
		return 1;
		}
		return 0;
		}
		}
		case STREAMER_TYPE_RACE_CP:
		{
		if (p->second.visibleRaceCheckpoint)
		{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(p->second.visibleRaceCheckpoint);
		if (serverWide || (r != core->getData()->raceCheckpoints.end() && r->second->amx == amx))
		{
		return 1;
		}
		}
		return 0;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
		if (serverWide)
		{
		return static_cast<cell>(p->second.internalMapIcons.size());
		}
		else
		{
		int count = 0;
		for (boost::unordered_map<int, int>::iterator m = p->second.internalMapIcons.begin(); m != p->second.internalObjects.end(); ++m)
		{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator n = core->getData()->mapIcons.find(m->first);
		if (n != core->getData()->mapIcons.end() && n->second->amx == amx)
		{
		++count;
		}
		}
		return static_cast<cell>(count);
		}
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
		if (serverWide)
		{
		return static_cast<cell>(p->second.internalTextLabels.size());
		}
		else
		{
		int count = 0;
		for (boost::unordered_map<int, int>::iterator t = p->second.internalTextLabels.begin(); t != p->second.internalTextLabels.end(); ++t)
		{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator u = core->getData()->textLabels.find(t->first);
		if (u != core->getData()->textLabels.end() && u->second->amx == amx)
		{
		++count;
		}
		}
		return static_cast<cell>(count);
		}
		}
		case STREAMER_TYPE_AREA:
		{
		if (serverWide)
		{
		return static_cast<cell>(p->second.internalAreas.size());
		}
		else
		{
		int count = 0;
		for (boost::unordered_set<int>::iterator a = p->second.internalAreas.begin(); a != p->second.internalAreas.end(); ++a)
		{
		boost::unordered_map<int, Item::SharedArea>::iterator b = core->getData()->areas.find(*a);
		if (b != core->getData()->areas.end() && b->second->amx == amx)
		{
		++count;
		}
		}
		return static_cast<cell>(count);
		}
		}
		default:
		{
		sampgdk::logprintf("*** Streamer_CountVisibleItems: Invalid type specified");
		return 0;
		}
		}
		}*/
		return 0;
	}

	int Streamer_DestroyAllItems(int type, int serverwide)
	{

		/*bool serverWide = static_cast<int>(params[2]) != 0;
		switch (static_cast<int>(params[1]))
		{
		case STREAMER_TYPE_OBJECT:
		{
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.begin();
		while (o != core->getData()->objects.end())
		{
		if (serverWide || o->second->amx == amx)
		{
		o = Utility::destroyObject(o);
		}
		else
		{
		++o;
		}
		}
		return 1;
		}
		case STREAMER_TYPE_PICKUP:
		{
		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.begin();
		while (p != core->getData()->pickups.end())
		{
		if (serverWide || p->second->amx == amx)
		{
		p = Utility::destroyPickup(p);
		}
		else
		{
		++p;
		}
		}
		return 1;
		}
		case STREAMER_TYPE_CP:
		{
		boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin();
		while (c != core->getData()->checkpoints.end())
		{
		if (serverWide || c->second->amx == amx)
		{
		c = Utility::destroyCheckpoint(c);
		}
		else
		{
		++c;
		}
		}
		return 1;
		}
		case STREAMER_TYPE_RACE_CP:
		{
		boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin();
		while (r != core->getData()->raceCheckpoints.end())
		{
		if (serverWide || r->second->amx == amx)
		{
		r = Utility::destroyRaceCheckpoint(r);
		}
		else
		{
		++r;
		}
		}
		return 1;
		}
		case STREAMER_TYPE_MAP_ICON:
		{
		boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.begin();
		while (m != core->getData()->mapIcons.end())
		{
		if (serverWide || m->second->amx == amx)
		{
		m = Utility::destroyMapIcon(m);
		}
		else
		{
		++m;
		}
		}
		return 1;
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
		boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.begin();
		while (t != core->getData()->textLabels.end())
		{
		if (serverWide || t->second->amx == amx)
		{
		t = Utility::destroyTextLabel(t);
		}
		else
		{
		++t;
		}
		}
		return 1;
		}
		case STREAMER_TYPE_AREA:
		{
		boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin();
		while (a != core->getData()->areas.end())
		{
		if (serverWide || a->second->amx == amx)
		{
		a = Utility::destroyArea(a);
		}
		else
		{
		++a;
		}
		}
		return 1;
		}
		default:
		{
		sampgdk::logprintf("*** Streamer_DestroyAllItems: Invalid type specified");
		return 0;
		}
		}*/
		return 0;
	}

	int Streamer_CountItems(int type, int serverwide)
	{

		/*bool serverWide = static_cast<int>(params[2]) != 0;
		switch (static_cast<int>(params[1]))
		{
		case STREAMER_TYPE_OBJECT:
		{
		if (serverWide)
		{
		return static_cast<cell>(core->getData()->objects.size());
		}
		else
		{
		int count = 0;
		for (boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.begin(); o != core->getData()->objects.end(); ++o)
		{
		if (o->second->amx == amx)
		{
		++count;
		}
		}
		return static_cast<cell>(count);
		}
		}
		case STREAMER_TYPE_PICKUP:
		{
		if (serverWide)
		{
		return static_cast<cell>(core->getData()->pickups.size());
		}
		else
		{
		int count = 0;
		for (boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.begin(); p != core->getData()->pickups.end(); ++p)
		{
		if (p->second->amx == amx)
		{
		++count;
		}
		}
		return static_cast<cell>(count);
		}
		}
		case STREAMER_TYPE_CP:
		{
		if (serverWide)
		{
		return static_cast<cell>(core->getData()->checkpoints.size());
		}
		else
		{
		int count = 0;
		for (boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin(); c != core->getData()->checkpoints.end(); ++c)
		{
		if (c->second->amx == amx)
		{
		++count;
		}
		}
		return static_cast<cell>(count);
		}
		}
		case STREAMER_TYPE_RACE_CP:
		{
		if (serverWide)
		{
		return static_cast<cell>(core->getData()->raceCheckpoints.size());
		}
		else
		{
		int count = 0;
		for (boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin(); r != core->getData()->raceCheckpoints.end(); ++r)
		{
		if (r->second->amx == amx)
		{
		++count;
		}
		}
		return static_cast<cell>(count);
		}
		}
		case STREAMER_TYPE_MAP_ICON:
		{
		if (serverWide)
		{
		return static_cast<cell>(core->getData()->mapIcons.size());
		}
		else
		{
		int count = 0;
		for (boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.begin(); m != core->getData()->mapIcons.end(); ++m)
		{
		if (m->second->amx == amx)
		{
		++count;
		}
		}
		return static_cast<cell>(count);
		}
		}
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
		if (serverWide)
		{
		return static_cast<cell>(core->getData()->textLabels.size());
		}
		else
		{
		int count = 0;
		for (boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.begin(); t != core->getData()->textLabels.end(); ++t)
		{
		if (t->second->amx == amx)
		{
		++count;
		}
		}
		return static_cast<cell>(count);
		}
		}
		case STREAMER_TYPE_AREA:
		{
		if (serverWide)
		{
		return static_cast<cell>(core->getData()->areas.size());
		}
		else
		{
		int count = 0;
		for (boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
		{
		if (a->second->amx == amx)
		{
		++count;
		}
		}
		return static_cast<cell>(count);
		}
		}
		default:
		{
		sampgdk::logprintf("*** Streamer_CountItems: Invalid type specified");
		return 0;
		}
		}*/
		return 0;
	}
};

namespace StreamerLibrary
{
	int CreateDynamicObject(int modelid, float x, float y, float z, float rx, float ry, float rz, int worldid, int interiorid, int playerid, float streamdistance, float drawdistance)
	{

		if (core->getData()->getMaxItems(STREAMER_TYPE_OBJECT) == core->getData()->objects.size())
		{
			return 0;
		}
		int objectID = Item::Object::identifier.get();
		Item::SharedObject object(new Item::Object);
		object->amx = &StreamerAMX;
		object->objectID = objectID;
		object->modelID = modelid;
		object->position = Eigen::Vector3f(x, y, z);
		object->rotation = Eigen::Vector3f(rx, ry, rz);
		Utility::addToContainer(object->worlds, worldid);
		Utility::addToContainer(object->interiors, interiorid);
		Utility::addToContainer(object->players, playerid);
		object->streamDistance = streamdistance * streamdistance;
		object->drawDistance = drawdistance;
		core->getGrid()->addObject(object);
		core->getData()->objects.insert(std::make_pair(objectID, object));
		return static_cast<cell>(objectID);
	}

	int DestroyDynamicObject(int objectid)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			Utility::destroyObject(o);
			return 1;
		}
		return 0;
	}

	bool IsValidDynamicObject(int objectid)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			return 1;
		}
		return 0;
	}

	int SetDynamicObjectPos(int objectid, float x, float y, float z)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			Eigen::Vector3f position = o->second->position;
			o->second->position = Eigen::Vector3f(x, y, z);
			for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
			{
				boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
				if (i != p->second.internalObjects.end())
				{
					SetPlayerObjectPos(p->first, i->second, o->second->position[0], o->second->position[1], o->second->position[2]);
				}
			}
			if (position[0] != o->second->position[0] || position[1] != o->second->position[1])
			{
				if (o->second->cell)
				{
					core->getGrid()->removeObject(o->second, true);
				}
			}
			if (o->second->move)
			{
				o->second->move->duration = static_cast<int>((static_cast<float>(boost::geometry::distance(o->second->move->position.get<0>(), o->second->position) / o->second->move->speed) * 1000.0f));
				o->second->move->position.get<1>() = o->second->position;
				o->second->move->position.get<2>() = (o->second->move->position.get<0>() - o->second->position) / static_cast<float>(o->second->move->duration);
				if ((o->second->move->rotation.get<0>().maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
				{
					o->second->move->rotation.get<1>() = o->second->rotation;
					o->second->move->rotation.get<2>() = (o->second->move->rotation.get<0>() - o->second->rotation) / static_cast<float>(o->second->move->duration);
				}
				o->second->move->time = boost::chrono::steady_clock::now();
			}
			return 1;
		}
		return 0;
	}

	int GetDynamicObjectPos(int objectid, float &x, float &y, float &z)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			if (o->second->move)
			{
				core->getStreamer()->processActiveItems();
			}
			x = o->second->position[0];
			y = o->second->position[1];
			z = o->second->position[2];
			return 1;
		}
		return 0;
	}

	int SetDynamicObjectRot(int objectid, float rx, float ry, float rz)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			o->second->rotation = Eigen::Vector3f(rx, ry, rz);
			for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
			{
				boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
				if (i != p->second.internalObjects.end())
				{
					SetPlayerObjectRot(p->first, i->second, o->second->rotation[0], o->second->rotation[1], o->second->rotation[2]);
				}
			}
			if (o->second->move)
			{
				if ((o->second->move->rotation.get<0>().maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
				{
					o->second->move->rotation.get<1>() = o->second->rotation;
					o->second->move->rotation.get<2>() = (o->second->move->rotation.get<0>() - o->second->rotation) / static_cast<float>(o->second->move->duration);
				}
			}
			return 1;
		}
		return 0;
	}

	int GetDynamicObjectRot(int objectid, float &rx, float &ry, float &rz)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			if (o->second->move)
			{
				core->getStreamer()->processActiveItems();
			}
			rx = o->second->rotation[0];
			ry = o->second->rotation[1];
			rz = o->second->rotation[2];
			return 1;
		}
		return 0;
	}

	int MoveDynamicObject(int objectid, float x, float y, float z, float speed, float rx, float ry, float rz)
	{

		if (!speed)
		{
			return 0;
		}
		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			if (o->second->attach)
			{
				sampgdk::logprintf("MoveDynamicObject: Object is currently attached and cannot be moved");
				return 0;
			}
			Eigen::Vector3f position(x, y, z);
			Eigen::Vector3f rotation(rx, ry, rz);
			o->second->move = boost::intrusive_ptr<Item::Object::Move>(new Item::Object::Move);
			o->second->move->duration = static_cast<int>((static_cast<float>(boost::geometry::distance(position, o->second->position) / speed) * 1000.0f));
			o->second->move->position.get<0>() = position;
			o->second->move->position.get<1>() = o->second->position;
			o->second->move->position.get<2>() = (position - o->second->position) / static_cast<float>(o->second->move->duration);
			o->second->move->rotation.get<0>() = rotation;
			if ((o->second->move->rotation.get<0>().maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
			{
				o->second->move->rotation.get<1>() = o->second->rotation;
				o->second->move->rotation.get<2>() = (rotation - o->second->rotation) / static_cast<float>(o->second->move->duration);
			}
			o->second->move->speed = speed;
			o->second->move->time = boost::chrono::steady_clock::now();
			for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
			{
				boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
				if (i != p->second.internalObjects.end())
				{
					StopPlayerObject(p->first, i->second);
					MovePlayerObject(p->first, i->second, o->second->move->position.get<0>()[0], o->second->move->position.get<0>()[1], o->second->move->position.get<0>()[2], o->second->move->speed, o->second->move->rotation.get<0>()[0], o->second->move->rotation.get<0>()[1], o->second->move->rotation.get<0>()[2]);
				}
			}
			core->getStreamer()->movingObjects.insert(o->second);
			return static_cast<cell>(o->second->move->duration);
		}
		return 0;
	}

	int StopDynamicObject(int objectid)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			if (o->second->move)
			{
				for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
				{
					boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
					if (i != p->second.internalObjects.end())
					{
						StopPlayerObject(p->first, i->second);
					}
				}
				o->second->move.reset();
				core->getStreamer()->movingObjects.erase(o->second);
				return 1;
			}
		}
		return 0;
	}

	bool IsDynamicObjectMoving(int objectid)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			if (o->second->move)
			{
				return 1;
			}
		}
		return 0;
	}

	int AttachCameraToDynamicObject(int playerid, int objectid)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectid);
			if (i != p->second.internalObjects.end())
			{
				AttachCameraToPlayerObject(p->first, i->second);
				return 1;
			}
		}
		return 0;
	}

	int AttachDynamicObjectToVehicle(int objectid, int vehicleid, float offsetx, float offsety, float offsetz, float rx, float ry, float rz)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			if (o->second->move)
			{
				sampgdk::logprintf("AttachDynamicObjectToVehicle: Object is currently moving and cannot be attached");
				return 0;
			}
			o->second->attach = boost::intrusive_ptr<Item::Object::Attach>(new Item::Object::Attach);
			o->second->attach->vehicle = vehicleid;
			o->second->attach->offset = Eigen::Vector3f(offsetx, offsety, offsetz);
			o->second->attach->rotation = Eigen::Vector3f(rx, ry, rz);
			for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
			{
				boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
				if (i != p->second.internalObjects.end())
				{
					AttachPlayerObjectToVehicle(p->first, i->second, o->second->attach->vehicle, o->second->attach->offset[0], o->second->attach->offset[1], o->second->attach->offset[2], o->second->attach->rotation[0], o->second->attach->rotation[1], o->second->attach->rotation[2]);
					for (boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.begin(); m != o->second->materials.end(); ++m)
					{
						if (m->second.main)
						{
							SetPlayerObjectMaterial(p->first, i->second, m->first, m->second.main->modelID, m->second.main->txdFileName.c_str(), m->second.main->textureName.c_str(), m->second.main->materialColor);
						}
						else if (m->second.text)
						{
							SetPlayerObjectMaterialText(p->first, i->second, m->second.text->materialText.c_str(), m->first, m->second.text->materialSize, m->second.text->fontFace.c_str(), m->second.text->fontSize, m->second.text->bold, m->second.text->fontColor, m->second.text->backColor, m->second.text->textAlignment);
						}
					}
				}
			}
			if (vehicleid != INVALID_GENERIC_ID)
			{
				core->getStreamer()->attachedObjects.insert(o->second);
			}
			else
			{
				o->second->attach.reset();
				core->getStreamer()->attachedObjects.erase(o->second);
				core->getGrid()->removeObject(o->second, true);
			}
			return 1;
		}
		return 0;
	}

	int EditDynamicObject(int playerid, int objectid)
	{

		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			core->getStreamer()->startManualUpdate(p->second, true);
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectid);
			if (i != p->second.internalObjects.end())
			{
				EditPlayerObject(p->first, i->second);
				return 1;
			}
		}
		return 0;
	}

	int GetDynamicObjectMaterial(int objectid, int materialindex, int &modelid, std::string &txdname, std::string &texturename, int &materialcolor)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.find(materialindex);
			if (m != o->second->materials.end())
			{
				if (m->second.main)
				{
					modelid = m->second.main->modelID;
					txdname = m->second.main->txdFileName;
					texturename = m->second.main->textureName;
					materialcolor = m->second.main->materialColor;
					return 1;
				}
			}
		}
		return 0;
	}

	int SetDynamicObjectMaterial(int objectid, int materialindex, int modelid, std::string txdname, std::string texturename, int materialcolor)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			int index = materialindex;
			o->second->materials[index].main = boost::intrusive_ptr<Item::Object::Material::Main>(new Item::Object::Material::Main);
			o->second->materials[index].main->modelID = modelid;
			o->second->materials[index].main->txdFileName = txdname;
			o->second->materials[index].main->textureName = texturename;
			o->second->materials[index].main->materialColor = materialcolor;
			for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
			{
				boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
				if (i != p->second.internalObjects.end())
				{
					SetPlayerObjectMaterial(p->first, i->second, index, o->second->materials[index].main->modelID, o->second->materials[index].main->txdFileName.c_str(), o->second->materials[index].main->textureName.c_str(), o->second->materials[index].main->materialColor);
				}
			}
			o->second->materials[index].text.reset();
			return 1;
		}
		return 0;
	}

	int GetDynamicObjectMaterialText(int objectid, int materialindex, std::string text, int &materialsize, std::string fontface, int &fontsize, int &bold, int &fontcolor, int &backcolor, int &textalignment)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.find(materialindex);
			if (m != o->second->materials.end())
			{
				if (m->second.text)
				{
					text = m->second.text->materialText;
					materialsize = m->second.text->materialSize;
					fontface = m->second.text->fontFace;
					fontsize = m->second.text->fontSize;
					bold = m->second.text->bold != 0;
					fontcolor = m->second.text->fontColor;
					backcolor = m->second.text->backColor;
					textalignment = m->second.text->textAlignment;
					return 1;
				}
			}
		}
		return 0;
	}

	int SetDynamicObjectMaterialText(int objectid, int materialindex, std::string text, int materialsize, std::string fontface, int fontsize, int bold, int fontcolor, int backcolor, int textalignment)
	{

		boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
		if (o != core->getData()->objects.end())
		{
			int index = materialindex;
			o->second->materials[index].text = boost::intrusive_ptr<Item::Object::Material::Text>(new Item::Object::Material::Text);
			o->second->materials[index].text->materialText = text;
			o->second->materials[index].text->materialSize = materialsize;
			o->second->materials[index].text->fontFace = fontface;
			o->second->materials[index].text->fontSize = fontsize;
			o->second->materials[index].text->bold = bold != 0;
			o->second->materials[index].text->fontColor = fontcolor;
			o->second->materials[index].text->backColor = backcolor;
			o->second->materials[index].text->textAlignment = textalignment;
			for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
			{
				boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
				if (i != p->second.internalObjects.end())
				{
					SetPlayerObjectMaterialText(p->first, i->second, o->second->materials[index].text->materialText.c_str(), index, o->second->materials[index].text->materialSize, o->second->materials[index].text->fontFace.c_str(), o->second->materials[index].text->fontSize, o->second->materials[index].text->bold, o->second->materials[index].text->fontColor, o->second->materials[index].text->backColor, o->second->materials[index].text->textAlignment);
				}
			}
			o->second->materials[index].main.reset();
			return 1;
		}
		return 0;
	}
};

namespace StreamerLibrary
{
	int CreateDynamicPickup(int modelid, int type, float x, float y, float z, int worldid, int interiorid, int playerid, float streamdistance)
	{

		if (core->getData()->getMaxItems(STREAMER_TYPE_PICKUP) == core->getData()->pickups.size())
		{
			return 0;
		}
		int pickupID = Item::Pickup::identifier.get();
		Item::SharedPickup pickup(new Item::Pickup);
		pickup->amx = &StreamerAMX;
		pickup->pickupID = pickupID;
		pickup->worldID = 0;
		pickup->modelID = modelid;
		pickup->type = type;
		pickup->position = Eigen::Vector3f(x, y, z);
		Utility::addToContainer(pickup->worlds, worldid);
		Utility::addToContainer(pickup->interiors, interiorid);
		Utility::addToContainer(pickup->players, playerid);
		pickup->streamDistance = streamdistance * streamdistance;
		core->getGrid()->addPickup(pickup);
		core->getData()->pickups.insert(std::make_pair(pickupID, pickup));
		return static_cast<cell>(pickupID);
	}

	int DestroyDynamicPickup(int pickupid)
	{

		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(pickupid);
		if (p != core->getData()->pickups.end())
		{
			Utility::destroyPickup(p);
			return 1;
		}
		return 0;
	}

	bool IsValidDynamicPickup(int pickupid)
	{

		boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(pickupid);
		if (p != core->getData()->pickups.end())
		{
			return 1;
		}
		return 0;
	}

	int GetDynamicObjectVirtualWorld(int objectid, int index)
	{
		//boost::unordered_map<int, Item::SharedObject>::iterator p = core->getData()->objects.find(objectid);
		//if (p != core->getData()->objects.end())
		//{
		//	auto it = p->second->worlds.begin();
		//	if (index)
		//		std::advance(it, index);
		//	return *it;
		//}
		return 0;
	}
	int GetDynamicObjectVirtualWorld(int objectid)
	{
		boost::unordered_map<int, Item::SharedObject>::iterator p = core->getData()->objects.find(objectid);
		if (p != core->getData()->objects.end())
		{
			return getFirstValueInContainer(p->second->worlds, 0);
		}
		return 0;
	}
};