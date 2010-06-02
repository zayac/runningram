/* 
 * File:   Body.cpp
 * Author: necto
 * 
 * Created on January 10, 2010, 3:39 PM
 */

#include "Car.h"
#include "Orient.h"
#include "Canvas.h"
#include "Sprite.h"
#include "Eventman.h"
#include "Player_manager.h"
#include "Effects_manager.h"

#ifndef max
inline float max (float a, float b) {return a < b ? b : a;}
#endif

Collision_vector Collis_rectangles (Vector2f one[4], Vector2f two[4], bool to_centras, bool recalled = false);

//
//Body::Body () { }
//
//Body::Body (const Body& orig) { }
//
Body::~Body () { }

float Car::max_health = 0;

//--------------------------------------------------------------------------------------------------
Car::Car (Vector2f coor, float health_, float motor_force_, float bouncy_, float angular_vel_, float rudder_spring_,
		  float rmass1, float rmass2, float len, float r1, float r2, float turn_transfer_, Vector2f fric, Orient start_orient, int id_,
		 Sprite* pic_ , Player* host_, Effects_manager* em_)
:Active (coor, len + max(r1, r2), id_), rp (0), lp (0), fp (0), bp (0), lenght (len), rmass (1/(1/rmass1 + 1/rmass2)),
 health (health_), motor_force (motor_force_), bouncy (bouncy_), angular_vel (angular_vel_),
 rudder_spring (rudder_spring_), host (host_), em (em_), pic (pic_), turn_transfer (turn_transfer_),
	back (rmass1, r1, coor - start_orient.getDir()*len/(1 + rmass1/rmass2), fric, start_orient),
	front (rmass2, r2, coor + start_orient.getDir()*len/(1 + rmass2/rmass1), fric, start_orient)
{
	if (health_ > max_health) max_health = health_;
	if (pic)
		setHeight (pic->getHeight ());
	else
		setHeight (r);
	assert(ok());
}
Car::~Car()
{
	if (em) em->createExplosion (pos, r);
}
//--------------------------------------------------------------------------------------------------
void Car::actions (float dt)
{
	assert(ok());
	front.applayMotion (dt);
	back.applayMotion (dt);

	Orient ori(front.pos - back.pos);

//	float fm = 1/front.rev_mass, bm = 1/back.rev_mass;

	pos = (back.pos/back.rev_mass + front.pos/front.rev_mass)*rmass;
	front.pos = pos + ori.getDir()*lenght/(1 + back.rev_mass/front.rev_mass);
	back.pos = pos - ori.getDir()*lenght/(1 + front.rev_mass/back.rev_mass);

	Orient delta = ori - back.orient;

	back.orient = ori;
	front.orient += delta.getAngle ()*(1 - turn_transfer);
	processGestures (dt);
	normalizeFrontOrient();

	rudderCorrection (dt);

}
Canvas* dbgcanv = 0;//deprecated;
bool collis_brd = true;
//--------------------------------------------------------------------------------------------------
void DBG_switch() { collis_brd = ! collis_brd;}
//--------------------------------------------------------------------------------------------------
void Car::draw (Canvas* c)
{
	back.orient.update ();
	Orient primary_o = back.orient;
	if (pic != 0)
	{
		float angle = PI + inRange(primary_o.getAngle() + PI/4);//PI/4 for isometric projection
		pic->draw (c, Canvas::transform (pos.to<int>()), angle/PI/2);
	}
//	else	//if car hasn't sprite, it will be drawn schematically
	{
		dbgcanv = c;//deprecated;
		assert(ok());
		Point begin = back.pos.to<int>();
		Point end = front.pos.to<int>();//(front.orient.Get_dir()*front.r + pos).To<int> ();

		Point leftback = Canvas::transform (Point (back.r*primary_o.getDir().y, -back.r*primary_o.getDir().x) + begin);
		Point leftfront = Canvas::transform (Point (front.r*primary_o.getDir().y, -front.r*primary_o.getDir().x) + end);
		Point rightback = Canvas::transform (Point (-back.r*primary_o.getDir().y, back.r*primary_o.getDir().x) + begin);
		Point rightfront = Canvas::transform (Point (-front.r*primary_o.getDir().y, front.r*primary_o.getDir().x) + end);

		Point one_trg = Canvas::transform (front.r*front.orient.getDir() + front.pos).to<int>();
		Point two_trg = Canvas::transform (front.r*(front.orient + Orient(2*PI/3)).getDir() + front.pos).to<int>();
		Point three_trg = Canvas::transform (front.r*(front.orient - Orient(2*PI/3)).getDir() + front.pos).to<int>();

		c->line (leftback, leftfront, Color (150, 200, 200));
		c->line (leftfront, rightfront, Color (150, 200, 200));
		c->line (rightfront, rightback, Color (150, 200, 200));
		c->line (rightback, leftback, Color (150, 200, 200));

		Color headc (200, 150, 150);

		c->line (one_trg, two_trg, headc);
		c->line (two_trg, three_trg, headc);
		c->line (three_trg, one_trg, headc);
		c->line (end, one_trg, headc);
	}
	if (!dead())
	{
		Color headc (200, 150, 150);
		int hlen = health/max_health*Health_indicator_len;
		Rect hline (pos.x - hlen/2, pos.y + Health_indicator_offset, hlen, Health_indicator_height);
		c->fillRect (hline, headc);
	}
}
//--------------------------------------------------------------------------------------------------
void Car::collisObj (Active* that)
{
    if (that->myType() == Car_type)
    {
        Car* op = static_cast<Car*> (that);

//        Vector2f one[4];
		Shape one;
        getMyVerticies (one.v);
		Shape two;
//        Vector2f two[4];
        op->getMyVerticies (two.v);
        Collision_vector cv = detectCollision (one, two, 0);//Collis_rectangles (one.v, two.v, false);
//		if (!cv.Vital()) cv = Detect_collision (one, two, 0);
        if (cv.vital())//There is an collision
            applayObjCollision (op, cv);
    }
}
//--------------------------------------------------------------------------------------------------
void Car::collisBrd (Rect with, float fric)
{
	if (!collis_brd) return;
	Vector2f lu (with.x, with.y);//left-up
	Vector2f ru (with.x + with.w, with.y);//right-up
	Vector2f ld (with.x, with.y + with.h);//left-down
	Vector2f rd (with.x + with.w, with.y + with.h);//right-down

	Vector2f one[4];
    getMyVerticies (one);
	Vector2f two[4] = {lu, ru, rd, ld};

	Collision_vector rez;
	Collision_vector curv = Collis_rectangles (one, two, false);
	rez.papp = curv.papp;
	rez.depth = curv.depth;

	int num_iters = 1;

	while (num_iters-- > 0 && curv.vital())
	{
		move (curv.depth);

        getMyVerticies (one);
		curv = Collis_rectangles (one, two, false);
		rez.papp = (rez.papp*rez.weight() + curv.papp*curv.weight())/(rez.weight() + curv.weight()); //averaging point of application
        rez.depth += curv.depth;
	}

    if (rez.vital()) applayBrdCollision (rez, fric);
}
//--------------------------------------------------------------------------------------------------
void Car::driveSand (Rect with, float fric)
{
	if (!collis_brd) return;
	Vector2f lu (with.x, with.y);//left-up
	Vector2f ru (with.x + with.w, with.y);//right-up
	Vector2f ld (with.x, with.y + with.h);//left-down
	Vector2f rd (with.x + with.w, with.y + with.h);//right-down

	Vector2f one[4];
    getMyVerticies (one);
	Vector2f two[4] = {lu, ru, rd, ld};

	Shape myf;
	getMyFrontVerticies (myf.v);
	myf.updateOrthos ();

	Shape myb;
	getMyBackVerticies (myb.v);
	myb.updateOrthos ();

	Shape sand;
	sand.v[0] = lu;
	sand.v[1] = ru;
	sand.v[2] = rd;
	sand.v[3] = ld;
	sand.updateOrthos ();

	Collision_vector rez  = detectCollision (myb, sand, 0);
    if (rez.vital()) applaySandFriction (rez, fric, Back_rect);

	rez  = detectCollision (myf, sand, 0);
    if (rez.vital()) applaySandFriction (rez, fric, Front_rect);
}
//--------------------------------------------------------------------------------------------------
void Car::getMyVerticies (Vector2f* four)
{
	four[0] = Vector2f (back.r*back.orient.getDir().y, -back.r*back.orient.getDir().x) + back.pos;
	four[1] = Vector2f (-back.r*back.orient.getDir().y, back.r*back.orient.getDir().x) + back.pos;
	four[2] = Vector2f (-front.r*back.orient.getDir().y, front.r*back.orient.getDir().x) + front.pos;
	four[3] = Vector2f (front.r*back.orient.getDir().y, -front.r*back.orient.getDir().x) + front.pos;
}
//--------------------------------------------------------------------------------------------------
void Car::getMyFrontVerticies (Vector2f* four)
{
	four[0] = Vector2f (front.r*back.orient.getDir().y, -front.r*back.orient.getDir().x) + (back.pos + front.pos)/2;
	four[1] = Vector2f (-front.r*back.orient.getDir().y, front.r*back.orient.getDir().x) + (back.pos + front.pos)/2;
	four[2] = four[0] + (front.pos - back.pos)/2;
	four[3] = four[1] + (front.pos - back.pos)/2;
}
//--------------------------------------------------------------------------------------------------
void Car::getMyBackVerticies (Vector2f* four)
{
	four[2] = Vector2f (back.r*back.orient.getDir().y, -back.r*back.orient.getDir().x) + (back.pos + front.pos)/2;
	four[3] = Vector2f (-back.r*back.orient.getDir().y, back.r*back.orient.getDir().x) + (back.pos + front.pos)/2;
	four[0] = four[2] - (front.pos - back.pos)/2;
	four[1] = four[3] - (front.pos - back.pos)/2;
}
//--------------------------------------------------------------------------------------------------
inline Vector2f From_point_to_stline (Vector2f end, Vector2f delta, Vector2f p)
{
	return (delta^(p-end))*delta/delta.lenSq() - (p - end);
}
inline bool Same_sign (float a, float b) {return a==0 ? b==0 : (a>0 ? b>0 : b<0);}
inline bool Same_side (Vector2f end, Vector2f delta, Vector2f a, Vector2f b)
{
	return Same_sign (delta.x*(a.y - end.y) - delta.y*(a.x- end.x),
					  delta.x*(b.y - end.y) - delta.y*(b.x- end.x));
}
#define IN(one, two, three, four, centre, what) Same_side (one, two-one, centre, what) &&		\
												Same_side (two, three-two, centre, what) &&		\
												Same_side (three, four-three, centre, what) &&	\
												Same_side (four, one-four, centre, what)
//-------------
inline Vector2f Get_depth (Vector2f rect[4], Vector2f centre, Vector2f p, Vector2f hiscentre, bool to_centre)
{
	Vector2f ret (1/aboutnull, 1/aboutnull);

	if (Same_side (rect[3], rect[0] - rect[3], centre, p))
	{
		ret = From_point_to_stline (rect[3], rect[0] - rect[3], p);
//		if (to_centre)
//		{
//			if ((ret^(p - hiscentre)) < 0) ret = Vector2f (1/aboutnull, 1/aboutnull);
//		}
	}

	for (int i = 0; i < 3; ++i)
		if (Same_side (rect[i], rect[i + 1] - rect[i], centre, p))
		{
			Vector2f cur = From_point_to_stline (rect[i], rect[i + 1] - rect[i], p);
			//(!to_centre || (ret^(p - hiscentre)) > 0) &&
			if (cur.lenSq() < ret.lenSq()) ret = cur;
		}
	return ret;
}
//-------------
inline bool On_line (const Vector2f& begin, const Vector2f& end, const Vector2f p)
{
	float delta = (p-begin).proj (end - begin).lenSq() - (p-begin).lenSq();
	return -aboutnull < delta && delta < aboutnull;
}
//-------------
inline bool On_segment (const Vector2f& begin, const Vector2f& end, const Vector2f p)
{
	return On_line (begin, end, p) && ((p-begin)^(end-begin)) > 0 && ((p-end)^(begin-end)) > 0;//between
}
//-------------
Collision_vector Collis_rectangles (Vector2f one[4], Vector2f two[4], bool to_centras, bool recalled)
{
	Vector2f cent1 = (one[0] + one[1] + one[2] + one[3])/4;
	Vector2f cent2 = (two[0] + two[1] + two[2] + two[3])/4;

	Collision_vector ret;

	for (int i = 0; i < 4; ++i)
		if (IN(two[0], two[1], two[2], two[3], cent2, one[i]))
		{
			Vector2f depth = Get_depth (two, cent2, one[i], cent1, to_centras);
			if (depth.lenSq() > ret.depth.lenSq())
			{
				ret.depth = depth;
				ret.papp = one[i];
			}
		}

	if (!recalled)
		for (int j = 0; j < 4; ++j)
			if (IN(one[0], one[1], one[2], one[3], cent1, two[j]))
				if (Get_depth (one, cent1, two[j], cent2, to_centras).lenSq() > ret.depth.lenSq())
				{
					ret = Collis_rectangles (two, one, to_centras, true);
					ret.depth = -ret.depth;
				}
	if (!ret.vital ())
	{
		//!!! Needed to avoie a combining two rects same size
//		if (On_segment (one[0], one[1], two[])) //tratata
	}
	return ret;
}
#undef IN
//--------------------------------------------------------------------------------------------------
void Car::applayBrdCollision (Collision_vector cv, float fric)
{
	if (dead()) return;
    Vector2f imp_along =  getImp (cv.papp).proj (cv.depth);

    if ((imp_along^cv.depth) < 0)
	{
        applImpulse (imp_along*(-1 - bouncy), cv.papp, 0.3);

		Vector2f vell = getVel (cv.papp);
		Vector2f vel_across = vell - vell.proj (cv.depth);

		applImpulse (-fric*vel_across*imp_along.lenSq (), cv.papp, 0.3);
		if (dead() && host != 0)
			host->subFrag ();
	}
}
//--------------------------------------------------------------------------------------------------
void Car::applayObjCollision (Car* with, Collision_vector cv)
{
	if (dead()) return;
	if (with->dead ()) return;
	float mass_sum = 1/rmass + 1/with->rmass;
	float rez_bouncy = bouncy*with->bouncy;
	assert (mass_sum > 0);
	Vector2f mydisp = cv.depth/rmass/mass_sum;
	Vector2f hisdisp = mydisp - cv.depth;
	
	with->move (hisdisp);
	move (mydisp);

	Vector2f my_imp = getImp (cv.papp).proj (cv.depth);
	Vector2f his_imp = with->getImp (cv.papp).proj (cv.depth);
	Vector2f along_dimp = (my_imp - his_imp);

	if ((along_dimp^cv.depth) < 0)
	{
		Vector2f ci_vel = (my_imp + his_imp)/mass_sum;//inertia center velocity

		Vector2f mydimp = (1 + rez_bouncy)*(ci_vel/rmass - my_imp);
		Vector2f hisdimp = (1 + rez_bouncy)*(ci_vel/with->rmass - his_imp);
		applImpulse (mydimp, cv.papp);
		with->applImpulse (hisdimp, cv.papp);

		if (with->dead() && host != 0)
			if (with->host == host) host->subFrag ();
			else					host->addFrag ();
		if (dead() && with->host != 0)						//frag encouting
			if (with->host == host) with->host->subFrag ();
			else					with->host->addFrag ();
	}
}
//--------------------------------------------------------------------------------------------------
void Car::applaySandFriction (Collision_vector cv, float fric, bool bORf)
{
	if (dead()) return;
    Vector2f imp = getImp (cv.papp);
	Vector2f resist;

	if (bORf == Back_rect)
		resist = back.getResist (getVel (cv.papp))*(cv.depth.lenSq ()/back.r/back.r);
	else
		resist = front.getResist (getVel (cv.papp))*(cv.depth.lenSq ()/front.r/front.r);

	resist *= fric;

	applForce (resist, cv.papp, true);
	if (dead() && host != 0)
		host->subFrag ();
}
//--------------------------------------------------------------------------------------------------
Vector2f Car::getVel (Vector2f p)
{
    Vector2f trans = back.vel;

    Vector2f axis = (front.pos - back.pos)/lenght;//normalize
    Vector2f dir = p - back.pos;
    float distance = dir.len();
    dir /= distance;                            //normalize

    Orient alfa = Orient (dir, true) - Orient (axis, true);

    Vector2f rotary = alfa.rotate (front.vel - back.vel)*distance/lenght;

    return trans + rotary;
}
//--------------------------------------------------------------------------------------------------
Vector2f Car::getImp (Vector2f p)
{
    Vector2f to_back = p - back.pos;
    Vector2f to_front = p - front.pos;

    Vector2f back_imp = back.getImpulse().proj (to_back);
    Vector2f front_imp = front.getImpulse().proj (to_front);

	return back_imp + front_imp;
}
//--------------------------------------------------------------------------------------------------
bool Car::damage (Vector2f imp, Vector2f papp, float destructive_k)
{
	Vector2f proj = (papp - pos).proj (back.orient.getDir());
	Vector2f norm = papp - pos - proj;
	float k = (2*norm.lenSq ()/r - ((papp - pos)^back.orient.getDir()) + r/2)/r;
	float val = imp.lenSq ()*k*destructive_k;

	if (dead()) return true;//crushed allready dead car

	health -= val;
	if (dead())
	{
		if (host) host->carCrushed ();
//		if (em) em->Create_explosion (pos, r); // this line moved to destructor, because client explosions
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------
bool Car::dead() const
{
	return health <= 0;
}
//--------------------------------------------------------------------------------------------------
void Car::die()
{
	health = -1;
}
//--------------------------------------------------------------------------------------------------
void Car::applImpulse (Vector2f imp, Vector2f papp, float destructive_k)
{
    Vector2f to_back = papp - back.pos;
    Vector2f to_front = papp - front.pos;
    float det = to_back.x*to_front.y - to_back.y*to_front.x;

    if (-aboutnull < det && det < aboutnull) //impulse along the cardan shaft
    {
		Vector2f dvel = imp*rmass;
		back.applImpulce (dvel/back.rev_mass);
		front.applImpulce (dvel/front.rev_mass);
        return;
    }

    Vector2f back_proj = to_back * (imp.x*to_front.y - imp.y*to_front.x)/det;
    Vector2f front_proj = to_front * (imp.y*to_back.x - imp.x*to_back.y)/det;// oblique projections

    Vector2f axis = front.pos - back.pos;
    Vector2f axis_bproj = (back_proj^axis)*axis/axis.lenSq();
    Vector2f axis_fproj = (front_proj^axis)*axis/axis.lenSq();

    Vector2f ortho_bproj = back_proj - axis_bproj;//this part takes only front
    Vector2f ortho_fproj = front_proj - axis_fproj;//this part takes only back


    Vector2f dvel_axis = (axis_bproj + axis_fproj)*rmass;//this part of impulse take both front and back

    back.applImpulce (dvel_axis/back.rev_mass + ortho_bproj);
    front.applImpulce (dvel_axis/front.rev_mass + ortho_fproj);

	damage (imp, papp, destructive_k);
}
//--------------------------------------------------------------------------------------------------
void Car::applForce (Vector2f f, Vector2f papp, bool resistancive)
{
    Vector2f to_back = papp - back.pos;
    Vector2f to_front = papp - front.pos;
    float det = to_back.x*to_front.y - to_back.y*to_front.x;

    if (-aboutnull < det && det < aboutnull) //impulse along the cardan shaft
    {
		Vector2f acc = f*rmass;
		back.applForce (acc/back.rev_mass, resistancive);
		front.applForce (acc/front.rev_mass, resistancive);
        return;
    }

    Vector2f back_proj = to_back * (f.x*to_front.y - f.y*to_front.x)/det;
    Vector2f front_proj = to_front * (f.y*to_back.x - f.x*to_back.y)/det;// oblique projections

    Vector2f axis = front.pos - back.pos;
    Vector2f axis_bproj = (back_proj^axis)*axis/axis.lenSq();
    Vector2f axis_fproj = (front_proj^axis)*axis/axis.lenSq();

    Vector2f ortho_bproj = back_proj - axis_bproj;//this part takes only front
    Vector2f ortho_fproj = front_proj - axis_fproj;//this part takes only back


    Vector2f acc_axis = (axis_bproj + axis_fproj)*rmass;//this part of impulse take both front and back

    back.applForce (acc_axis/back.rev_mass + ortho_bproj, resistancive);
    front.applForce (acc_axis/front.rev_mass + ortho_fproj, resistancive);
}
//--------------------------------------------------------------------------------------------------
int Car::signDataLen() const
{
	return Active::signDataLen () + sizeof (back) + sizeof (front) + sizeof (rmass) + sizeof (lenght) +
		sizeof (health) + sizeof (motor_force) + sizeof (bouncy) +
		sizeof (angular_vel) + sizeof (rudder_spring);
}
//--------------------------------------------------------------------------------------------------
int Car::exp (char* buffer, int size) const
{
	int disp = Active::exp (buffer, size);
	if (disp == -1) return -1;
	if (signDataLen () > size) return -1;

#define WRITE(field) memcpy (buffer + disp, &field, sizeof(field)); disp += sizeof(field);

	WRITE(back)
	WRITE(front)
	WRITE(rmass)
	WRITE(lenght)
	WRITE(health)
	WRITE(motor_force)
	WRITE(bouncy)
	WRITE(angular_vel)
	WRITE(rudder_spring)
#undef WRITE
	return disp;
}
//--------------------------------------------------------------------------------------------------
int Car::imp (char* buffer, int size)
{
	int disp = Active::imp (buffer, size);
	if (disp == -1) return -1;
	if (signDataLen () > size) return -1;

#define READ(field) memcpy (&field, buffer + disp, sizeof(field)); disp += sizeof(field);

	READ(back)
	READ(front)
	READ(rmass)
	READ(lenght)
	READ(health)
	READ(motor_force)
	READ(bouncy)
	READ(angular_vel)
	READ(rudder_spring)
#undef READ
	return disp;
}
//--------------------------------------------------------------------------------------------------
void Car::processGestures (float dt)
{
	assert(ok());
	if (fp && !bp) motoryForce ( motor_force);
	if (bp && !fp) motoryForce (-motor_force);
	if (rp && !lp) turnFront ( angular_vel*dt);
	if (lp && !rp) turnFront (-angular_vel*dt);
}
//--------------------------------------------------------------------------------------------------
void Car::motoryForce (float f)
{
	assert(ok());
	Vector2f accl = f*rmass*back.orient.getDir ();
	back.applForce (accl/back.rev_mass);
	front.applForce (accl/front.rev_mass);
}
//--------------------------------------------------------------------------------------------------
void Car::turnFront (float dfi)
{
	assert(ok());
//	Orient delta = front.orient - back.orient;
//	if (delta + dfi > Max_ang_dev) dfi = Max_ang_dev - delta;
//	if (delta + dfi < -Max_ang_dev) dfi = -Max_ang_dev - delta;

	front.orient += dfi;
}
//--------------------------------------------------------------------------------------------------
void Car::normalizeFrontOrient()
{
	assert(ok());
	float delta = inRange ((front.orient - back.orient).getAngle ());
	if (delta > Max_ang_dev) front.orient += Max_ang_dev - delta;
	if (delta < -Max_ang_dev) front.orient += -Max_ang_dev - delta;
}
//--------------------------------------------------------------------------------------------------
void Car::rudderCorrection (float dt)
{
	assert(ok());
	float delta = inRange ((front.orient - back.orient).getAngle ());
	front.orient = back.orient + delta*(1 - rudder_spring*dt);
}
//--------------------------------------------------------------------------------------------------
inline bool Boolly (bool arg) {return arg == true || arg == false;}
bool Car::ok() const
{
	return Boolly (fp) && Boolly (bp) && Boolly (lp) && Boolly (rp) &&
		back.ok () && front.ok () && lenght > 0 && rmass > 0;
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
int Active::exp (char* buffer, int size) const
{
	if (sizeof (pos) + sizeof (r) > size) return -1;
	
	memcpy (buffer, &pos, sizeof(pos));
	buffer += sizeof(pos);
	memcpy (buffer, &r, sizeof(r));
	
	return sizeof (pos) + sizeof (r);
}
//--------------------------------------------------------------------------------------------------
int Active::imp (char* buffer, int size)
{
	if (sizeof (pos) + sizeof (r) > size) return -1;
	
	memcpy (&pos, buffer, sizeof (pos));
	buffer += sizeof (pos);
	memcpy (&r, buffer, sizeof (buffer));
	
	return sizeof (pos) + sizeof (r);
}
//--------------------------------------------------------------------------------------------------