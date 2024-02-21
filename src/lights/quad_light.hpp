
#ifndef __RT_ISICG_QUAD_LIGHT__
#define __RT_ISICG_QUAD_LIGHT__

#include "base_light.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	class QuadLight : public BaseLight
	{
	  public:
		QuadLight()			  = delete;
		virtual ~QuadLight() = default;

		QuadLight( const Vec3f & p_position,
				   const Vec3f & p_u,
				   const Vec3f & p_v,
				   const Vec3f & p_color,
				   const float	 p_power )
			: BaseLight( p_color, p_power, true ), _position( p_position ), _u( p_u ), _v( p_v )
		{
			_normal = glm::normalize(glm::cross( _u, _v ));
			_area = glm::length(_u) * glm::length(_v);
		}

		inline const Vec3f & getPosition() const { return _position; }
		inline const Vec3f & getU() const { return _u; }
		inline const Vec3f & getV() const { return _v; }
		inline const Vec3f & getNormal() const { return _normal; }
		inline const float getArea() const { return _area; }

		virtual LightSample sample( const Vec3f & p_point ) const override
		{
			// LightSample(const Vec3f & p_direction, const float p_distance, const Vec3f & p_radiance, const float
			// p_pdf )
			Vec3f pointInQuad = this->getPosition() + randomFloat() * this->getU() + randomFloat() * this->getV();
			Vec3f direction = glm::normalize( pointInQuad - p_point);
			float distance = glm::distance( pointInQuad, p_point );

			float pdf = 1.f / getArea() * glm::pow( distance, 2 ) / glm::cos( glm::dot(this->getNormal(), direction ));
			Vec3f radiance = this->getFlatColor() * this->getPower() / pdf;
			return LightSample( direction, distance, radiance, 1.f );
		}

	  protected:
		Vec3f _position = VEC3F_ZERO;
		Vec3f _u = VEC3F_ZERO;
		Vec3f _v = VEC3F_ZERO;
		Vec3f _normal	= VEC3F_ZERO;
		float _area = 0.f;	
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_LIGHT__