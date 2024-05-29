#ifndef __VECTORTWOD_H__
#define __VECTORTWOD_H__

//------------------------------------------------------------------//
//- class VECTOR2D_INT ---------------------------------------------//
//------------------------------------------------------------------//
//- A class for a two dimensional vector composed of an array of 2 -//
//- int variables.												   -//
//------------------------------------------------------------------//
class VECTOR2D_INT
	{
	public:
		int v[2];

	const VECTOR2D_INT operator=  (const VECTOR2D_INT&	vec);
	const VECTOR2D_INT operator+  (const VECTOR2D_INT&	vec);
	const VECTOR2D_INT operator+  (void);
	const VECTOR2D_INT operator+= (const VECTOR2D_INT&	vec);
	const VECTOR2D_INT operator-  (const VECTOR2D_INT&	vec);
	const VECTOR2D_INT operator-  (void);
	const VECTOR2D_INT operator-= (const int&	vec);
	const VECTOR2D_INT operator*= (const int&	s);
	const VECTOR2D_INT operator/= (const int&	s);
	const VECTOR2D_INT operator*  (const int&   s);
	const VECTOR2D_INT operator*  (const VECTOR2D_INT&  vec);
	const VECTOR2D_INT operator/  (int s);
//	const VECTOR2D_INT operator=  (const VECTOR2D_FLOAT&	vec);
//	const VECTOR2D_INT operator+  (const VECTOR2D_FLOAT&	vec);
//	const VECTOR2D_INT operator+= (const VECTOR2D_FLOAT&	vec);
//	const VECTOR2D_INT operator-  (const VECTOR2D_FLOAT&	vec);
	const VECTOR2D_INT operator-= (const float&	vec);
	const VECTOR2D_INT operator*= (const float&	s);
	const VECTOR2D_INT operator/= (const float&	s);
	const VECTOR2D_INT operator*  (const float&   s);
//	const VECTOR2D_INT operator*  (const VECTOR2D_FLOAT&  vec);
	const VECTOR2D_INT operator/  (float s);
	const bool		   operator== (const VECTOR2D_INT&  vec);
	const bool		   operator!= (const VECTOR2D_INT&  vec);
//	const bool		   operator== (const VECTOR2D_FLOAT&  vec);
//	const bool		   operator!= (const VECTOR2D_FLOAT&  vec);

	void SetProperties(int X, int Y);

	VECTOR2D_INT(void)
		{	memset(v, 0, sizeof(float[2]));	}
	VECTOR2D_INT(int X, int Y)
		{
		v[0]= X;
		v[1]= Y;
		}

	~VECTOR2D_INT(void)
		{	}
	};

#endif /*__VECTORTWOD_H__*/