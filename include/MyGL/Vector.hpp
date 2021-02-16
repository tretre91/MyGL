#ifndef MY_VECTOR
#define MY_VECTOR

#include "mygl_export.h"

namespace my
{

    /**
     * @brief A class for manipulating 2 component integer vectors
    */
    class MYGL_EXPORT Vec2 {

    public:
        int x;
        int y;

        /**
         * @brief Construcs a 2 component vector initialized to 0
        */
        Vec2();

        /**
         * @brief Creates a 2 component vector
         * @param x The x (first) component
         * @param y The y (second) component
        */
        Vec2(int x, int y);

        Vec2 operator+(const Vec2& vec);
        Vec2& operator+=(const Vec2& vec);
        Vec2 operator-(const Vec2& vec);
        Vec2& operator-=(const Vec2& vec);

        Vec2 operator*(int scalar);
        Vec2& operator*=(int scalar);
    };


    /**
     * @brief A class for manipulating 3 component integer vectors
    */
    class MYGL_EXPORT Vec3 {

    public:
        int x;
        int y;
        int z;

        /**
         * @brief Construcs a 3 component vector initialized to 0
        */
        Vec3();

        /**
         * @brief @brief Creates a 3 component vector
         * @param x The x (first) component
         * @param y The y (second) component
         * @param z The z (third) component
        */
        Vec3(int x, int y, int z);

        Vec3 operator+(const Vec3& vec);
        Vec3& operator+=(const Vec3& vec);
        Vec3 operator-(const Vec3& vec);
        Vec3& operator-=(const Vec3& vec);

        Vec3 operator*(int scalar);
        Vec3& operator*=(int scalar);
    };


    /**
     * @brief A class for manipulating 4 component integer vectors
    */
    class MYGL_EXPORT Vec4 {

    public:
        int x;
        int y;
        int z;
        int w;

        /**
         * @brief Construcs a 4 component vector initialized to 0
        */
        Vec4();

        /**
         * @brief @brief Creates a 4 component vector
         * @param x The x (first) component
         * @param y The y (second) component
         * @param z The z (third) component
         * @param w The w (fourth) component
        */
        Vec4(int x, int y, int z, int w);

        Vec4 operator+(const Vec4& vec);
        Vec4& operator+=(const Vec4& vec);
        Vec4 operator-(const Vec4& vec);
        Vec4& operator-=(const Vec4& vec);

        Vec4 operator*(int scalar);
        Vec4& operator*=(int scalar);
    };

}


#endif //MY_VECTOR