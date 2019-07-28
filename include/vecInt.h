class vec3Int{
public:
	int x;
	int y;
	int z;

	vec3Int(int x, int y, int z):x(x),y(y),z(z){}
	vec3Int(int c = 0):x(c),y(c),z(c){}

	void rotateZ(short direction){
		int _x=x;
		x = -direction*y;
		y = direction*_x;
	}

	void rotateY(short direction){
		int _x=x;
		x = direction*z;
		z = -direction*_x;
	}

	void rotateX(short direction){
		int _y = y;
		y = -direction*z;
		z = direction*_y;
	}
};