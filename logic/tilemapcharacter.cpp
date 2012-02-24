#include "tilemapcharacter.h"
#include "graphics/color.h"

TilemapCharacter::TilemapCharacter(Tilemap &parent)
	: siz(0,0), cen(0,0),
	  m_acc(0,0), m_vel(0,0), m_fri(0,0), m_velLim(0,0),
	  m_animVelFactor(1.0), m_parent(parent)
{

}

void TilemapCharacter::update(float deltaTime)
{
	using math::vec2f;
	vec2f pos0 = m_pos; //pos0: posicion inicial

	//uniformly accelerated linear motion, posf: posicion final
	vec2f posf = pos0 + m_vel*deltaTime + m_acc*deltaTime*deltaTime*0.5f;
	m_vel = m_vel + m_acc*deltaTime;

	//FRICTION
	if (m_fri.x != 0)
	{
		if (m_vel.x < 0)
		{
			m_vel.x += m_fri.x*deltaTime;
			if (m_vel.x > 0) m_vel.x = 0;
		}
		else if (m_vel.x > 0)
		{
			m_vel.x -= m_fri.x*deltaTime;
			if (m_vel.x < 0) m_vel.x = 0;
		}

	}
	if (m_fri.y != 0)
	{
		if (m_vel.y < 0)
		{
			m_vel.y += m_fri.y*deltaTime;
			if (m_vel.y > 0) m_vel.y = 0;
		}
		else if (m_vel.x > 0)
		{
			m_vel.y -= m_fri.y*deltaTime;
			if (m_vel.y < 0) m_vel.y = 0;
		}

	}

	//Hacemos clamp de las velocidades
	if (m_vel.x >  m_velLim.x) m_vel.x =  m_velLim.x;
	if (m_vel.x < -m_velLim.x) m_vel.x = -m_velLim.x;
	if (m_vel.y >  m_velLim.y) m_vel.y =  m_velLim.y;
	if (m_vel.y < -m_velLim.y) m_vel.y = -m_velLim.y;


	//Obtenemos el vector direccion para saber hacia donde nos dirigimos

	// El funcionamiento de las colisiones es el siguiente:
	// Aun que tengamos una posicion continua del personaje tenemos posiciones discretas
	// donde mirar si hay tiles que colisionan o no. El algoritmo va por pasos, primero
	// abanzaremos en la coordenada Y, y luego en la X.
	//
	// Cuando abancemos en la Y, tendremos una Y_origen y una Y_destino, y
	// ademas el personajes es posible que este ocupando un rango de varias X
	// si es lo suficiente ancho o simplemente esta entre dos tiles, por tanto
	// tambien tenemos unas X_izquierda y X_derecha.
	// Ahora lo unico que hay que hacer es, para cada y que hay entre Y_origen
	// e Y_destino, y para cada x entre X_izquierda y X_derecha miramos si en
	// la posicion x,y del tilemap hay un tile colisionable. Si lo hay es que
	// nuestro personaje se va a chochar.
	vec2f scen = siz-cen;
	vec2f direction = posf - pos0;
	if (direction.y < 0) //Vamos hacia abajo
	{
		//le restamos a la Y la mitad de su tamaño para obtener la Y inferior del sprite
		int yo = m_parent.tilePosY(pos0.y - scen.y),
		    yn = m_parent.tilePosY(posf.y - scen.y),
		    xl = m_parent.tilePosX(pos0.x -  cen.x + 2),
		    xr = m_parent.tilePosX(pos0.x + scen.x - 2);
		for (int y = yo; y >= yn; y--)
		{
			for (int x = xl; x <= xr; x++)
			{
				if (m_parent.isColl(x,y) && onDownCollision(x, y))
				{
					posf.y = m_parent.Top(y) + scen.y;
					goto vert_exit;
				}
			}
		}

		noDownCollision();
	}
	else if (direction.y > 0) //Vamos hacia arriba
	{
		//le sumamos a la Y la mitad de su tamaño para obtener la Y superior del sprite
		int yo = m_parent.tilePosY(pos0.y +  cen.y),
		    yn = m_parent.tilePosY(posf.y +  cen.y),
		    xl = m_parent.tilePosX(pos0.x -  cen.x + 2),
		    xr = m_parent.tilePosX(pos0.x + scen.x - 2);
		for (int y = yo; y <= yn; y++)
		{
			for (int x = xl; x <= xr; x++)
			{
				if (m_parent.isColl(x,y) && onUpCollision(x, y))
				{
					posf.y = m_parent.Bottom(y) - cen.y;
					goto vert_exit;	
				}
			}
		}

		noUpCollision();
	}
	vert_exit:

	if (direction.x < 0) //Vamos hacia la izquierda
	{
		int xo = m_parent.tilePosX(pos0.x -  cen.x),
		    xn = m_parent.tilePosX(posf.x -  cen.x),
		    yb = m_parent.tilePosY(pos0.y - scen.y + 2),
		    yt = m_parent.tilePosY(pos0.y +  cen.y - 2);
		for (int x = xo; x >= xn; x--)
		{
			for (int y = yb; y <= yt; y++)
			{
				if (m_parent.isColl(x,y) && onLeftCollision(x, y))
				{
					posf.x = m_parent.Right(x) + cen.x;
					goto horz_exit;
				}
			}
		}

		noLeftCollision();
	}
	else if (direction.x > 0) //Vamos hacia la derecha
	{
		int xo = m_parent.tilePosX(pos0.x + scen.x),
		    xn = m_parent.tilePosX(posf.x + scen.x),
		    yb = m_parent.tilePosY(pos0.y - scen.y + 2),
		    yt = m_parent.tilePosY(pos0.y +  cen.y - 2);
		for (int x = xo; x <= xn; x++)
		{
			for (int y = yb; y <= yt; y++)
			{
				if (m_parent.isColl(x,y) && onRightCollision(x, y))
				{
					posf.x = m_parent.Left(x) - scen.x;
					goto horz_exit;	
				}
			}
		}

		noRightCollision();
	}
	horz_exit:

	m_pos = posf; //asignamos la posicion final a pos
	SpriteAnim::update(deltaTime * m_animVelFactor);
}

void TilemapCharacter::noLeftCollision() {}
void TilemapCharacter::noRightCollision(){}
void TilemapCharacter::noUpCollision()   {}
void TilemapCharacter::noDownCollision() {}

bool TilemapCharacter::onLeftCollision(int x, int j) {return true;}
bool TilemapCharacter::onRightCollision(int x, int j){return true;}
bool TilemapCharacter::onUpCollision(int x, int j)   {return true;}
bool TilemapCharacter::onDownCollision(int x, int j) {return true;}

bool TilemapCharacter::ensureAnim(std::string name)
{
	if (name != m_lastAnim)
	{
		SelectAnim(name);
		m_lastAnim = name;

		Sprite::drawParams params;
		getParamsToDraw(params);
		siz.x = params.w;
		siz.y = params.h;
		cen.x = params.cx;
		cen.y = params.cy;
		return true;
	}

	return false;
}

void TilemapCharacter::preDrawing()
{
	glColor(rgba(1,1,1,1));
}
