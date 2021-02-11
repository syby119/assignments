#include "./print_info.hpp"

void unitTestQuaternion(bool enable = false) {
	if (!enable) {
		return;
	}

	printf("========= Quaternion Unit Test =========");

	printf("\n****************** class member function *****************\n");

	/* default constructor */ {
		printf("\ndefault constructor: identity quaternion\n");
		Quaternion<float> q;
		print(q);
	}

	/* constructor with w, x, y, z*/ {
		printf("\nconstructor with 4 values\n");
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
		print(q);
	}

	/* copy constructor */ {
		printf("\ncopy constructor\n");
		Quaternion<float> q = Quaternion<float>(1.0f, 2.0f, 3.0f, 4.0f);
		print(q);
	}

	/* move constructor */ {
		printf("\nno explicitly defined move constructor \n");
	}

	/* operator = */ {
		printf("\noperator=\n");
		Quaternion<float> q;
		q = Quaternion<float>(1.0f, 2.0f, 3.0f, 4.0f);
		print(q);
	}

	/* operator += */ {
		printf("\noperator+=\n");
		Quaternion<float> q;
		q += Quaternion<float>(1.0f, 2.0f, 3.0f, 4.0f);
		print(q);
	}

	/* operator -= */ {
		printf("\noperator-=\n");
		Quaternion<float> q;
		q -= Quaternion<float>(1.0f, 2.0f, 3.0f, 4.0f);
		print(q);
	}

	/* operator *= (scaler)*/ {
		printf("\nq *= c\n");
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
		q *= 2.0f;
		print(q);
	}

	/* operator *= (quaternion)*/ {
		printf("\nq *= p\n");
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
		Quaternion<float> p(5.0f, 9.0f, 8.0f, 7.0f);
		q *= p;
		print(q);
	}

	/* operator /= c*/ {
		printf("\nq /= c\n");
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
		q /= 2.0f;
		print(q);
	}

	printf("\n********************* global function *************************\n");
	/* operator + (unary)*/ {
		printf("\n+q\n");
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
		print(+q);
	}

	/* operator - (unary)*/ {
		printf("\n-q\n");
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
		print(-q);
	}

	/* operator + */ {
		printf("\np + q\n");
		Quaternion<float> p(1.0f, 2.0f, 3.0f, 4.0f);
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
		print(p + q);
	}

	/* operator - */ {
		printf("\np - q\n");
		Quaternion<float> p(1.0f, 2.0f, 3.0f, 4.0f);
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
		print(p - q);
	}

	/* operator * (scaler * quaternion) */ {
		printf("\nc * q\n");
		Quaternion<float> p(1.0f, 2.0f, 3.0f, 4.0f);
		print(2.0f * p);
	}

	/* operator * (quaternion * scaler) */ {
		printf("\nq * c\n");
		Quaternion<float> p(1.0f, 2.0f, 3.0f, 4.0f);
		print(p * 2.0f);
	}

	/* operator * (quaternion * quaternion) */ {
		printf("\np * q\n");
		Quaternion<float> p(1.0f, 2.0f, 3.0f, 4.0f);
		Quaternion<float> q(5.0f, 9.0f, 8.0f, 7.0f);

		print(p * q);
	}

	/* operator / (quaternion / scaler) */ {
		printf("\nq / c\n");
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);

		print(q / 2.0f);
	}

	/* norm */ {
		printf("\nnorm(q)\n");
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);

		printf("%f\n", norm(q));
	}

	/* norm2 */ {
		printf("\nnorm2(q)\n");
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);

		printf("%f\n", norm2(q));
	}

	/* conjugate */ {
		printf("\nconjugate(q)\n");
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);

		print(conjugate(q));
	}

	/* inverse */ {
		printf("\ninverse(q)\n");
		Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);

		print(inverse(q));
	}
}

void unitTestVector3(bool enable = false) {
	if (!enable) {
		return;
	}

	printf("========= Vector3 Unit Test =========");

	printf("\n****************** class member function *****************\n");

	/* default constructor */ {
		printf("\ndefault constructor: zero vector3\n");
		Vector3<float> v;
		print(v);
	}

	/* constructor with 3 values*/ {
		printf("\nconstructor with 3 values\n");
		Vector3<float> v(1.0f, 2.0f, 3.0f);
		print(v);
	}

	/* copy constructor */ {
		printf("\ncopy constructor\n");
		Vector3<float> u(1.0f, 2.0f, 3.0f);
		Vector3<float> v = u;
		print(v);
	}
	
	/* move constructor */ {
		printf("\nno explicitly defined move constructor \n");
	}

	/* operator = */ {
		printf("\noperator = \n");
		Vector3<float> v;
		Vector3<float> u(1.0f, 2.0f, 3.0f);
		v = u;
		print(v);
	}

	/* operator[] */ {
		printf("\nv[i] = c\n");
		Vector3<float> v;
		v[0] = 1.0f;
		v[1] = 2.0f;
		v[2] = 3.0f;
		print(v);
	}

	/* const operator[] */ {
		printf("\nv[i]\n");
		const Vector3<float> v(1.0f, 2.0f, 3.0f);
		print(v);
	}

	/* operator += */ {
		printf("\noperator += \n");
		Vector3<float> v(1.0f, 2.0f, 3.0f);
		Vector3<float> u(1.0f, 2.0f, 3.0f);
		v += u;
		print(v);
	}

	/* operator -= */ {
		printf("\noperator -= \n");
		Vector3<float> v(1.0f, 2.0f, 3.0f);
		Vector3<float> u(1.0f, 2.0f, 3.0f);
		v -= u;
		print(v);
	}

	/* operator *= */ {
		printf("\noperator *= \n");
		Vector3<float> v(1.0f, 2.0f, 3.0f);
		v *= 2.0f;
		print(v);
	}

	/* operator /= */ {
		printf("\noperator /= \n");
		Vector3<float> v(1.0f, 2.0f, 3.0f);
		v /= 2.0f;
		print(v);
	}

	/* v.dot(u) */ {
		printf("\nv.dot(u)\n");
		Vector3<float> v(1.0f, 2.0f, 3.0f);
		Vector3<float> u(3.0f, 2.0f, 1.0f);
		printf("%f\n", v.dot(u));
	}

	/* v.cross(u) */ {
		printf("\nv.cross(u)\n");
		Vector3<float> v(1.0f, 2.0f, 3.0f);
		Vector3<float> u(3.0f, 2.0f, 1.0f);
		print(v.cross(u));
	}

	printf("\n********************* global function *************************\n");

	/* operator+ (unary)*/ {
		printf("\n+v\n");
		Vector3<float> v(1.0f, 2.0f, 3.0f);
		print(+v);
	}


	/* operator- (unary)*/ {
		printf("\n-v\n");
		Vector3<float> v(1.0f, 2.0f, 3.0f);
		print(-v);
	}

	/* operator+ */ {
		printf("\nu + v\n");
		Vector3<float> u(1.0f, 2.0f, 3.0f);
		Vector3<float> v(3.0f, 2.0f, 1.0f);
		print(u + v);
	}

	/* operator- */ {
		printf("\nu - v\n");
		Vector3<float> u(1.0f, 2.0f, 3.0f);
		Vector3<float> v(3.0f, 2.0f, 1.0f);
		print(u - v);
	}

	/* operator*(scaler * vector3) */ {
		printf("\nc * v\n");
		Vector3<float> v(1.0f, 2.0f, 3.0f);
		print(2.0f * v);
	}

	/* operator*(vector3 * scaler) */ {
		printf("\nv * c\n");
		Vector3<float> v(1.0f, 2.0f, 3.0f);
		print(v * 2.0f);
	}

	/* operator/ */ {
		printf("\nv / c\n");
		Vector3<float> v(1.0f, 2.0f, 3.0f);
		print(v / 2.0f);
	}

	/* dot(u, v) */ {
		printf("\ndot(u, v)\n");
		Vector3<float> u(1.0f, 2.0f, 3.0f);
		Vector3<float> v(3.0f, 2.0f, 1.0f);
		printf("%f\n", dot(u, v));
	}

	/* cross(u, v) */ {
		printf("\ncross(u, v)\n");
		Vector3<float> u(1.0f, 2.0f, 3.0f);
		Vector3<float> v(3.0f, 2.0f, 1.0f);
		print(cross(u, v));
	}
}

void unitTestVector4(bool enable = false) {
	if (!enable) {
		return;
	}

	printf("========= Vector4 Unit Test =========");

	printf("\n****************** class member function *****************\n");

	/* default constructor */ {
		printf("\ndefault constructor: zero vector4\n");
		Vector4<float> v;
		print(v);
	}

	/* constructor with 4 values*/ {
		printf("\nconstructor with 3 values\n");
		Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);
		print(v);
	}

	/* copy constructor */ {
		printf("\ncopy constructor\n");
		Vector4<float> u(1.0f, 2.0f, 3.0f, 4.0f);
		Vector4<float> v = u;
		print(v);
	}

	/* move constructor */ {
		printf("\nno explicitly defined move constructor \n");
	}

	/* operator = */ {
		printf("\noperator = \n");
		Vector4<float> v;
		Vector4<float> u(1.0f, 2.0f, 3.0f, 4.0f);
		v = u;
		print(v);
	}

	/* operator[] */ {
		printf("\nv[i] = c\n");
		Vector4<float> v;
		v[0] = 1.0f;
		v[1] = 2.0f;
		v[2] = 3.0f;
		v[3] = 4.0f;
		print(v);
	}

	/* const operator[] */ {
		printf("\nv[i]\n");
		const Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);
		print(v);
	}

	/* operator += */ {
		printf("\noperator += \n");
		Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);
		Vector4<float> u(1.0f, 2.0f, 3.0f, 4.0f);
		v += u;
		print(v);
	}

	/* operator -= */ {
		printf("\noperator -= \n");
		Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);
		Vector4<float> u(1.0f, 2.0f, 3.0f, 4.0f);
		v -= u;
		print(v);
	}

	/* operator *= */ {
		printf("\noperator *= \n");
		Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);
		v *= 2.0f;
		print(v);
	}

	/* operator /= */ {
		printf("\noperator /= \n");
		Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);
		v /= 2.0f;
		print(v);
	}

	/* v.dot(u) */ {
		printf("\nv.dot(u)\n");
		Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);
		Vector4<float> u(4.0f, 3.0f, 2.0f, 1.0f);
		printf("%f\n", v.dot(u));
	}

	printf("\n********************* global function *************************\n");

	/* operator+ (unary)*/ {
		printf("\n+v\n");
		Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);
		print(+v);
	}


	/* operator- (unary)*/ {
		printf("\n-v\n");
		Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);
		print(-v);
	}

	/* operator+ */ {
		printf("\nu + v\n");
		Vector4<float> u(1.0f, 2.0f, 3.0f, 4.0f);
		Vector4<float> v(4.0f, 3.0f, 2.0f, 1.0f);
		print(u + v);
	}

	/* operator- */ {
		printf("\nu - v\n");
		Vector4<float> u(1.0f, 2.0f, 3.0f, 4.0f);
		Vector4<float> v(4.0f, 3.0f, 2.0f, 1.0f);
		print(u - v);
	}

	/* operator*(scaler * vector3) */ {
		printf("\nc * v\n");
		Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);
		print(2.0f * v);
	}

	/* operator*(vector3 * scaler) */ {
		printf("\nv * c\n");
		Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);
		print(v * 2.0f);
	}

	/* operator/ */ {
		printf("\nv / c\n");
		Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);
		print(v / 2.0f);
	}

	/* dot(u, v) */ {
		printf("\ndot(u, v)\n");
		Vector4<float> u(1.0f, 2.0f, 3.0f, 4.0f);
		Vector4<float> v(4.0f, 3.0f, 2.0f, 1.0f);
		printf("%f\n", dot(u, v));
	}
}

void unitTestMatrix3x3(bool enable = false) {
	if (!enable) {
		return;
	}

	printf("========= Matrix3x3 Unit Test =========");
	
	printf("\n****************** class member function *****************\n");
	/* default constructor */ {
		printf("\ndefault constructor: zero matrix\n");
		Matrix3x3<float> m;
		print(m);
	}

	/* constructor with one scaler */ {
		printf("\nconstructor with one scaler: diagonal matrix\n");
		Matrix3x3<float> m(1.0f);
		print(m);
	}

	/* constructor with 3x3 elements */ {
		printf("\nconstructor with 3x3 elements: column major storage\n");
		Matrix3x3<float> m(
			1.0f, 2.0f, 3.0f,
			4.0f, 5.0f, 6.0f,
			7.0f, 8.0f, 9.0f);
		print(m);
		printf("contiguous inner storage: \n");
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				printf("m%d%d = %f\n", j, i, m[i][j]);
			}
		}
	}

	/* constructor with 3 vector */ {
		printf("\nconstructor with 3 column vector\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		print(m);
	}

	/* copy constructor */ {
		printf("\ncopy constructor\n");
		Matrix3x3<float> t(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));

		Matrix3x3<float> m = t;

		print(m);
	}

	/* move constructor */ {
		printf("\nno explicitly defined move constructor \n");
	}

	/* operator = */ {
		printf("\nm = n\n");
		Matrix3x3<float> m;

		Matrix3x3<float> n(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));

		m = n;

		print(m);
	}

	/* operator += */ {
		printf("\nm += n\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		
		Matrix3x3<float> n(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));

		m += n;

		print(m);
	}

	/* operator -= */ {
		printf("\nm -= n\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		
		Matrix3x3<float> n(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		
		m -= n;

		print(m);
	}

	/* operator *=(scaler) */ {
		printf("\nm *= c\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		m *= 2.0f;

		print(m);
	}

	/* operator *=(Matrix3x3) */ {
		printf("\nm *= n\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));

		Matrix3x3<float> n(
			Vector3<float>(9.0f, 8.0f, 7.0f),
			Vector3<float>(6.0f, 5.0f, 4.0f),
			Vector3<float>(3.0f, 2.0f, 1.0f));

		m *= n;

		print(m);
	}

	/* operator /=(scaler) */ {
		printf("\nm /= c\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));

		m /= 2.0f;

		print(m);
	}

	/* operator /=(Matrix3x3) */ {
		printf("\nm /= n\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));

		Matrix3x3<float> n(
			Vector3<float>(4.0f, 2.0f, 1.0f),
			Vector3<float>(2.0f, 0.0f, 4.0f),
			Vector3<float>(9.0f, 4.0f, 2.0f));

		m /= n;

		print(m);
	}

	/* determinant */ {
		printf("\nm.determinate()\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		printf("%f\n", m.determinant());
	}

	/* transpose */ {
		printf("\nm.transpose()\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		m.transpose();
		print(m);
	}

	/* inverse */ {
		printf("\nm.inverse()\n");

		Matrix3x3<float> m(
			Vector3<float>(4.0f, 2.0f, 1.0f),
			Vector3<float>(2.0f, 0.0f, 4.0f),
			Vector3<float>(9.0f, 4.0f, 2.0f));
		m.inverse();
		//print(m);
		print(m);
	}

	printf("\n********************* global function *************************\n");

	/* operator + (unary)*/ {
		printf("\n+m\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		print(+m);
	}


	/* operator - (unary)*/ {
		printf("\n-m\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		print(-m);
	}

	/* operator + */ {
		printf("\nm + n\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		Matrix3x3<float> n(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		print(m + n);
	}

	/* operator - */ {
		printf("\nm - n\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		Matrix3x3<float> n(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		print(m - n);
	}

	/* operator *(scaler) */ {
		printf("\nc * m\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));

		print(2.0f * m);
	}

	/* operator *(scaler) */ {
		printf("\nm * c\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));

		print(m * 2.0f);
	}

	/* operator *(Vector3) */ {
		printf("\nm * v\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));

		Vector3<float> v = Vector3<float>(1.0f, 2.0f, 3.0f);

		print(m * v);
	}

	/* operator *(Matrix3x3) */ {
		printf("\nm * n\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		Matrix3x3<float> n(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		print(m * n);
	}

	/* operator /(Matrix3x3 / scaler ) */ {
		printf("\nm / c\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));
		print(m / 2.0f);
	}

	/* operator /(scaler / Matrix3x3) */ {
		printf("\nc / m\n");
		Matrix3x3<float> m(
			Vector3<float>(4.0f, 2.0f, 1.0f),
			Vector3<float>(2.0f, 0.0f, 4.0f),
			Vector3<float>(9.0f, 4.0f, 2.0f));
		print(2.0f / m);
	}

	/* operator /(Matrix3x3) */ {
		printf("\nm / n\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));

		Matrix3x3<float> n(
			Vector3<float>(4.0f, 2.0f, 1.0f),
			Vector3<float>(2.0f, 0.0f, 4.0f),
			Vector3<float>(9.0f, 4.0f, 2.0f));

		print(m / n);
	}

	/* determinant */ {
		printf("\ndet(m)\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));

		printf("%f\n", determinant(m));
	}

	/* transpose */ {
		printf("\ntranspose(m)\n");
		Matrix3x3<float> m(
			Vector3<float>(1.0f, 2.0f, 3.0f),
			Vector3<float>(4.0f, 5.0f, 6.0f),
			Vector3<float>(7.0f, 8.0f, 9.0f));

		print(transpose(m));
	}

	/* inverse */ {
		printf("\ninverse(m)\n");
		Matrix3x3<float> m(
			Vector3<float>(4.0f, 2.0f, 1.0f),
			Vector3<float>(2.0f, 0.0f, 4.0f),
			Vector3<float>(9.0f, 4.0f, 2.0f));

		print(inverse(m));
	}
}

void unitTestMatrix4x4(bool enable = false) {
	if (!enable) {
		return;
	}

	printf("========= Matrix4x4 Unit Test =========");

	printf("\n****************** class member function *****************\n");
	/* default constructor */ {
		printf("\ndefault constructor: zero matrix\n");
		Matrix4x4<float> m;
		print(m);
	}

	/* constructor with one scaler */ {
		printf("\nconstructor with one scaler: diagonal matrix\n");
		Matrix4x4<float> m(1.0f);
		print(m);
	}

	/* constructor with 4x4 elements */ {
		printf("\nconstructor with 4x4 elements: column major storage\n");
		Matrix4x4<float> m(
			1.0f, 2.0f, 3.0f, 4.0f, 
			5.0f, 6.0f, 7.0f, 8.0f,
			9.0f, 10.0f, 11.0f, 12.0f,
			13.0f, 14.0f, 15.0f, 16.0f);
		print(m);
		printf("contiguous inner storage: \n");
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				printf("m%d%d = %f\n", j, i, m[i][j]);
			}
		}
	}

	/* constructor with 4 vector */ {
		printf("\nconstructor with 4 column vector\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));
		print(m);
	}

	/* copy constructor */ {
		printf("\ncopy constructor\n");
		Matrix4x4<float> n(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		Matrix4x4<float> m = n;

		print(m);
	}

	/* move constructor */ {
		printf("\nno explicitly defined move constructor \n");
	}

	/* operator = */ {
		printf("\nm = n\n");
		Matrix4x4<float> m;

		Matrix4x4<float> n(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		m = n;

		print(m);
	}

	/* operator += */ {
		printf("\nm += n\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		Matrix4x4<float> n(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		m += n;

		print(m);
	}

	/* operator -= */ {
		printf("\nm -= n\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		Matrix4x4<float> n(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		m -= n;

		print(m);
	}

	/* operator *=(scaler) */ {
		printf("\nm *= c\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));
		m *= 2.0f;

		print(m);
	}

	/* operator *=(Matrix4x4) */ {
		printf("\nm *= n\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		Matrix4x4<float> n(
			Vector4<float>(16.0f, 15.0f, 14.0f, 13.0f),
			Vector4<float>(12.0f, 11.0f, 10.0f, 9.0f),
			Vector4<float>(8.0f, 7.0f, 6.0f, 5.0f),
			Vector4<float>(4.0f, 3.0f, 2.0f, 1.0f));

		m *= n;

		print(m);
	}

	/* operator /=(scaler) */ {
		printf("\nm /= c\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		m /= 2.0f;

		print(m);
	}

	/* operator /=(Matrix4x4) */ {
		printf("\nm /= n\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		Matrix4x4<float> n(
			Vector4<float>(1.0f, 2.0f, 3.0f, 1.0f),
			Vector4<float>(1.0f, 2.0f, 4.0f, 3.0f),
			Vector4<float>(2.0f, 0.0f, 2.0f, 5.0f),
			Vector4<float>(1.0f, 1.0f, 1.0f, 27.0f));

		m /= n;

		print(m);
	}

	/* determinant */ {
		printf("\nm.determinate()\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 1.0f),
			Vector4<float>(1.0f, 2.0f, 4.0f, 3.0f),
			Vector4<float>(2.0f, 0.0f, 2.0f, 5.0f),
			Vector4<float>(1.0f, 1.0f, 1.0f, 27.0f));
		printf("%f\n", m.determinant());
	}

	/* transpose */ {
		printf("\nm.transpose()\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));
		m.transpose();
		print(m);
	}

	/* inverse */ {
		printf("\nm.inverse()\n");

		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 1.0f),
			Vector4<float>(1.0f, 2.0f, 4.0f, 3.0f),
			Vector4<float>(2.0f, 0.0f, 2.0f, 5.0f),
			Vector4<float>(1.0f, 1.0f, 1.0f, 27.0f));

		m.inverse();
		print(m);
	}

	printf("\n********************* global function *************************\n");

	/* operator + (unary)*/ {
		printf("\n+m\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));
		print(+m);
	}

	/* operator - (unary)*/ {
		printf("\n-m\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));
		print(-m);
	}

	/* operator + */ {
		printf("\nm + n\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));
		Matrix4x4<float> n(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));
		print(m + n);
	}

	/* operator - */ {
		printf("\nm - n\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));
		Matrix4x4<float> n(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));
		print(m - n);
	}

	/* operator *(scaler) */ {
		printf("\nc * m\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		print(2.0f * m);
	}

	/* operator *(scaler) */ {
		printf("\nm * c\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		print(m * 2.0f);
	}

	/* operator *(Vector4) */ {
		printf("\nm * v\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		Vector4<float> v(1.0f, 2.0f, 3.0f, 4.0f);

		print(m * v);
	}

	/* operator *(Matrix4x4) */ {
		printf("\nm * n\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));
		Matrix4x4<float> n(
			Vector4<float>(16.0f, 15.0f, 14.0f, 13.0f),
			Vector4<float>(12.0f, 11.0f, 10.0f, 9.0f),
			Vector4<float>(8.0f, 7.0f, 6.0f, 5.0f),
			Vector4<float>(4.0f, 3.0f, 2.0f, 1.0f));
		print(m * n);
	}

	/* operator /(Matrix4x4 / scaler ) */ {
		printf("\nm / c\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));
		print(m / 2.0f);
	}

	/* operator /(scaler / Matrix4x4) */ {
		printf("\nc / m\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 1.0f),
			Vector4<float>(1.0f, 2.0f, 4.0f, 3.0f),
			Vector4<float>(2.0f, 0.0f, 2.0f, 5.0f),
			Vector4<float>(1.0f, 1.0f, 1.0f, 27.0f));
		print(2.0f / m);
	}

	/* operator /(Matrix4x4) */ {
		printf("\nm / n\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));
		Matrix4x4<float> n(
			Vector4<float>(1.0f, 2.0f, 3.0f, 1.0f),
			Vector4<float>(1.0f, 2.0f, 4.0f, 3.0f),
			Vector4<float>(2.0f, 0.0f, 2.0f, 5.0f),
			Vector4<float>(1.0f, 1.0f, 1.0f, 27.0f));

		print(m / n);
	}

	/* determinant */ {
		printf("\ndet(m)\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		printf("%f\n", determinant(m));
	}

	/* transpose */ {
		printf("\ntranspose(m)\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f),
			Vector4<float>(5.0f, 6.0f, 7.0f, 8.0f),
			Vector4<float>(9.0f, 10.0f, 11.0f, 12.0f),
			Vector4<float>(13.0f, 14.0f, 15.0f, 16.0f));

		print(transpose(m));
	}

	/* inverse */ {
		printf("\ninverse(m)\n");
		Matrix4x4<float> m(
			Vector4<float>(1.0f, 2.0f, 3.0f, 1.0f),
			Vector4<float>(1.0f, 2.0f, 4.0f, 3.0f),
			Vector4<float>(2.0f, 0.0f, 2.0f, 5.0f),
			Vector4<float>(1.0f, 1.0f, 1.0f, 27.0f));

		print(inverse(m));
	}

}

int main() {
	unitTestQuaternion();

	unitTestVector3();

	unitTestVector4();
	
	unitTestMatrix3x3();

	unitTestMatrix4x4();

	return 0;
}