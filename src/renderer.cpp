#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "integrators/direct_lighting_integrator.hpp"
#include "integrators/whitted_integrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"
#include <stdlib.h>
#include <glm/common.hpp>

namespace RT_ISICG
{
	Renderer::Renderer() { _integrator = new RayCastIntegrator(); }

	void Renderer::setIntegrator( const IntegratorType p_integratorType )
	{
		if ( _integrator != nullptr ) { delete _integrator; }

		switch ( p_integratorType )
		{
		case IntegratorType::RAY_CAST:
		{
			_integrator = new RayCastIntegrator();
			break;
		}
		case IntegratorType::DIRECT_LIGHTING: 
		{
			_integrator = new DirectLightingIntegrator();
			break;
		}
		case IntegratorType::WHITTED:
		{ 
			_integrator = new WhittedIntegrator(); 
			break;
		}
		}



	}

	void Renderer::setBackgroundColor( const Vec3f & p_color )
	{
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl; }
		else
		{
			_integrator->setBackgroundColor( p_color );
		}
	}

	float Renderer::renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture )
	{
		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();

		Chrono			   chrono;
		ConsoleProgressBar progressBar;

		progressBar.start( height, 50 );
		chrono.start();

		#pragma omp parallel for
		for( int j = 0; j < height; j++ )
		{
			#pragma omp parallel for
			for( int i = 0; i < width; i++ )
			{
				/// TODO !
				//Vec3f color = Vec3f( float( i ) / ( width - 1 ), float(j) / ( height - 1 ), 0.f );
				Vec3f color = VEC3F_ZERO;
				Vec3f colorFinal;
				for( int k = 0; k < _nbPixelSamples; k++ ) 
				{
					Ray rayon = p_camera->generateRay( float( i + randomFloat() ) / ( width ),
													   float( j + randomFloat() ) / ( height ) );
					//Ray rayon = p_camera->generateRay( float( i ) / ( width - 1 ), float( j ) / ( height - 1 ) );

					color += _integrator->Li( p_scene, rayon, 0, 1000 );

					// p_texture.setPixel( i, j,  (rayon.getDirection() + 1.f) * 0.5f);
					
				}
				colorFinal = color / (float) _nbPixelSamples; 
				p_texture.setPixel( i, j,glm::clamp( colorFinal, 0.f, 1.f) );


			}
			progressBar.next();
		}

		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}
} // namespace RT_ISICG
