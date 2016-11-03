class Brick
{
public:
	Brick();
	Brick(int type, int tile, int hits, bool destroyable);

	int const getType(){ return type; }
	int const getTile(){ return tile; }
	int const gethits(){ return hits; }
	int const isDestroyable(){ return destroyable; }

	void hit();
	void setTile(int tile);
private:
	unsigned int hits;
	int type;
	int tile;
	bool destroyable;

};

