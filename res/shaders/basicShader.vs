#version 330

attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;
attribute vec3 weights;
attribute vec2 texCoord;


out vec2 texCoord0;
out vec3 normal0;
out vec3 color0;
out vec3 position0;



uniform mat4 MVP;
uniform mat4 Normal;

uniform mat4 preNormal;
uniform mat4 scale;
uniform vec4 prequaternion;
uniform vec4 myquaternion;
uniform vec4 nextquaternion;

uniform vec4 prequaterniondual;
uniform vec4 myquaterniondual;
uniform vec4 nextquaterniondual;


void main()
{
	texCoord0 = texCoord;
	//texCoord0 = vec2(0.0, 0.0);
	color0 = color;

	//normal0 = (Normal * vec4(normal, 0.0)).xyz;
	//position0 = vec3(Normal * vec4(position, 1.0));
	//gl_Position = MVP * vec4(position, 1.0) ; //you must have gl_Position
	
	//vec4 real2 = vec4( myquaternion );
	//vec4 dual2 = vec4(myquaterniondual );
	
	//vec4 real2 = vec4(weights.x * prequaternion + (weights.y+ weights.z ) * myquaternion );
	//vec4 dual2 = vec4(weights.x * prequaterniondual + (weights.y+ weights.z )* myquaterniondual );

	//vec4 real2 = vec4((weights.x +weights.y) * myquaternion + weights.z *nextquaternion);
	//vec4 dual2 = vec4((weights.x +weights.y) * myquaterniondual + weights.z *nextquaterniondual);
	vec4 real2 = vec4(weights.x * prequaternion + weights.y * myquaternion + weights.z * nextquaternion);
	vec4 dual2 = vec4(weights.x * prequaterniondual + weights.y * myquaterniondual + weights.z * nextquaterniondual);
	//vec4 real = vec4( prequaternion );
	//vec4 dual = vec4(prequaterniondual );
	
	float nt0 = 2*(-dual2.w*real2.x+dual2.x*real2.w-dual2.y*real2.z+dual2.z*real2.y);
	float nt1= 2*(-dual2.w*real2.y + dual2.x *real2.z + dual2.y*real2.w - dual2.z*real2.x);
	float nt2= 2*(-dual2.w*real2.z - dual2.x*real2.y + dual2.y*real2.x + dual2.z*real2.w);

	//float t0 = 2*(-dual.w*real.x+dual.x*real.w-dual.y*real.z+dual.z*real.y);
	//float t1= 2*(-dual.w*real.y + dual.x *real.z + dual.y*real.w - dual.z*real.x);
	//float t2= 2*(-dual.w*real.z - dual.x*real.y + dual.y*real.x + dual.z*real.w);



	

	//--------
	mat4 basicMat2 ;
	mat4 transposmat2;
	vec4 myPosition0 = vec4(1.0 - 2*real2.y*real2.y - 2*real2.z*real2.z, 2*real2.x*real2.y - 2*real2.w*real2.z, 2*real2.x*real2.z + 2*real2.w*real2.y, nt0);
	vec4 myPosition1 = vec4(2*real2.x*real2.y + 2*real2.w*real2.z, 1.0 - 2*real2.x*real2.x - 2*real2.z*real2.z, 2*real2.y*real2.z - 2*real2.w*real2.x, nt1);
	vec4 myPosition2 = vec4(2*real2.x*real2.z - 2*real2.w*real2.y, 2*real2.y*real2.z + 2*real2.w*real2.x, 1.0 - 2*real2.x*real2.x - 2*real2.y*real2.y, nt2);
	vec4 myPosition3 = vec4(0,0,0,1);


	basicMat2[0] = myPosition0;
	basicMat2[1] = myPosition1;
	basicMat2[2] = myPosition2;
	basicMat2[3] = myPosition3;

	//temp[0] = vec3(myPosition0.x,myPosition0.y,myPosition0.z);
	//temp[1] = vec3(myPosition1.x,myPosition1.y,myPosition1.z);
	//temp[2] = vec3(myPosition2.x,myPosition2.y,myPosition2.z);

	transposmat2[0] = vec4(myPosition0.x,myPosition1.x,myPosition2.x,0.0);
	transposmat2[1] = vec4(myPosition0.y,myPosition1.y,myPosition2.y,0.0);
	transposmat2[2] = vec4(myPosition0.z,myPosition1.z,myPosition2.z,0.0);
	transposmat2[3] = vec4(nt0,nt1,nt2,1.0);
	//---
	normal0 = (transposmat2 * vec4(normal, 0.0)).xyz;

	position0 = vec3(transposmat2 * (vec4(position, 1.0)));

	//gl_Position = (MVP *transposmat2* (vec4(position,1.0)))  ; 
	gl_Position = MVP * transposmat2   * vec4(position, 1.0) ; //you must have gl_Position

}
