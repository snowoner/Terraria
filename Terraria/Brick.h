class Brick
{
public:
	Brick();
	Brick(int type, int hits);

	int const getType(){ return type; }
	int const gethits(){ return hits; }

	void hit();
	void setType(int type);
private:
	unsigned int hits;
	int type;

};

