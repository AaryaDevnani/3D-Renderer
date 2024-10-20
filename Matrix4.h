#include "Vector4.h";
#include "Vector3.h"

class Matrix4 {
	
public: 
	// use 4 vector 4s to represent a 4x4 matrix
	Vector4 m_matrix[4];
	
	Matrix4() {
		m_matrix[0] = Vector4(1, 0, 0, 0);
		m_matrix[1] = Vector4(0, 1, 0, 0);
		m_matrix[2] = Vector4(0, 0, 1, 0);
		m_matrix[3] = Vector4(0, 0, 0, 1);
	}

	Matrix4(const Matrix4& copy) {
		m_matrix[0] = copy.m_matrix[0];
		m_matrix[1] = copy.m_matrix[1];
		m_matrix[2] = copy.m_matrix[2];
		m_matrix[3] = copy.m_matrix[3];
	}

	Matrix4(Vector4 v0, Vector4 v1, Vector4 v2, Vector4 v3) {
		m_matrix[0] = v0;
		m_matrix[1] = v1;
		m_matrix[2] = v2;
		m_matrix[3] = v3;
	}

	Matrix4(Vector3 v0, Vector3 v1, Vector3 v2) {
		m_matrix[0] = Vector4(v0.getX(), v0.getY(), v0.getZ(), 0);
		m_matrix[1] = Vector4(v1.getX(), v1.getY(), v1.getZ(), 0);
		m_matrix[2] = Vector4(v2.getX(), v2.getY(), v2.getZ(), 0);
		m_matrix[3] = Vector4(0, 0, 0, 1);
	}

	Vector4 getRow(int i) const {
		return m_matrix[i];
	}

	Vector4 getColumn(int i) const {
		if (i < 0 || i > 3) {
			// return error
			return Vector4();
		}
		switch (i)
		{
		case 0:
			return Vector4(m_matrix[0].getX(), m_matrix[1].getX(), m_matrix[2].getX(), m_matrix[3].getX());
		case 1:
			return Vector4(m_matrix[0].getY(), m_matrix[1].getY(), m_matrix[2].getY(), m_matrix[3].getY());
		case 2:
			return Vector4(m_matrix[0].getZ(), m_matrix[1].getZ(), m_matrix[2].getZ(), m_matrix[3].getZ());
		case 3:
			return Vector4(m_matrix[0].getW(), m_matrix[1].getW(), m_matrix[2].getW(), m_matrix[3].getW());
		default:
			break;
		}
	}

	void setColumn(int i, Vector4 v) {
		if (i < 0 || i > 3) {
			// return error
			return;
		}
		switch (i)
		{
		case 0:
			m_matrix[0].set(0, v.getX());
			m_matrix[1].set(0, v.getY());
			m_matrix[2].set(0, v.getZ());
			m_matrix[3].set(0, v.getW());
			break;
		case 1:
			m_matrix[0].set(1, v.getX());
			m_matrix[1].set(1, v.getY());
			m_matrix[2].set(1, v.getZ());
			m_matrix[3].set(1, v.getW());
			break;
		case 2:
			m_matrix[0].set(2, v.getX());
			m_matrix[1].set(2, v.getY());
			m_matrix[2].set(2, v.getZ());
			m_matrix[3].set(2, v.getW());
			break;
		case 3:
			m_matrix[0].set(3, v.getX());
			m_matrix[1].set(3, v.getY());
			m_matrix[2].set(3, v.getZ());
			m_matrix[3].set(3, v.getW());
			break;
		default:
			break;
		}
	}
    Matrix4 operator * (const Matrix4& mat) const {
        Matrix4 result;

        // Loop through rows of the first matrix
        for (int i = 0; i < 4; i++) {
            // Loop through columns of the second matrix
			Vector4 v = Vector4(0, 0, 0, 0);
			for (int j = 0; j < 4; j++) {
				// Calculate the dot product of the ith row of this matrix and the jth column of the mat
				float cij = m_matrix[i].dotProduct(mat.getColumn(j));
				v.set(j, cij);
			};
			result.m_matrix[i] = v;

        }

        return result;
    }

	Vector4 operator * (const Vector4& vec) const {
		Vector4 result;

		// Loop through rows of the matrix
		for (int i = 0; i < 4; i++) {
			// Compute dot product of the ith row of the matrix and the vector
			result.set(i, m_matrix[i].dotProduct(vec));

		}

		return result;
	}


	Matrix4 removeTranslation() {
		Matrix4 result = *this;
		result.setColumn(3, Vector4(0, 0, 0, 1));
		return result;
	}
	Matrix4 removeScaling() {
		//Remove any scaling(normalize columns)
		Matrix4 result = *this;
		Vector4 v0 = result.getColumn(0);
		Vector4 v1 = result.getColumn(1);
		Vector4 v2 = result.getColumn(2);
		//Vector4 v3 = result.getColumn(3);
		v0 = v0.normalize();
		v1 = v1.normalize();
		v2 = v2.normalize();
		//v3 = v3.normalize();
		result.setColumn(0, v0);
		result.setColumn(1, v1);
		result.setColumn(2, v2);
		//result.setColumn(3, v3);
		return result;

	}



	Vector4 multVec4 (const Vector4& vec) const {
		Vector4 result;

		// Loop through rows of the matrix
		for (int i = 0; i < 4; i++) {
			// Compute dot product of the ith row of the matrix and the vector
			result.set(i, m_matrix[i].dotProduct(vec));

		}

		return result;
	}


    static Matrix4 convertFromGzMat(GzMatrix mat) {
        // convert GzMatrix to Matrix4
        Vector4 v0(mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
        Vector4 v1(mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
        Vector4 v2(mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
        Vector4 v3(mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
        return Matrix4(v0, v1, v2, v3);
    }

    static void convertToGzMat(const Matrix4& mat, GzMatrix& result) {
        // convert Matrix4 to GzMatrix
        result[0][0] = mat.getRow(0).getX();
        result[0][1] = mat.getRow(0).getY();
        result[0][2] = mat.getRow(0).getZ();
        result[0][3] = mat.getRow(0).getW();

        result[1][0] = mat.getRow(1).getX();
        result[1][1] = mat.getRow(1).getY();
        result[1][2] = mat.getRow(1).getZ();
        result[1][3] = mat.getRow(1).getW();

        result[2][0] = mat.getRow(2).getX();    
        result[2][1] = mat.getRow(2).getY();
        result[2][2] = mat.getRow(2).getZ();
        result[2][3] = mat.getRow(2).getW();

        result[3][0] = mat.getRow(3).getX();
        result[3][1] = mat.getRow(3).getY();
        result[3][2] = mat.getRow(3).getZ();
        result[3][3] = mat.getRow(3).getW();
    }

	Matrix4 setIdentity() {
		// set the matrix to identity matrix
		m_matrix[0] = Vector4(1, 0, 0, 0);
		m_matrix[1] = Vector4(0, 1, 0, 0);
		m_matrix[2] = Vector4(0, 0, 1, 0);
		m_matrix[3] = Vector4(0, 0, 0, 1);

	}
};