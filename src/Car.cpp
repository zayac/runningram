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
		  float rmass1, float rmass2, float len, float r1, float r2, Vector2f fric, Orient start_orient,
		 Sprite* pic_ , Player* host_)
:Active (coor, len + max(r1, r2)), rp(0), lp(0), fp(0), bp(0), lenght (len), rmass (1/(1/rmass1 + 1/rmass2)),
	health (health_), motor_force (motor_force_), bouncy (bouncy_), angular_vel (angular_vel_),
	rudder_spring (rudder_spring_), host (host_), pic(pic_),
back (rmass1, r1, coor - start_orient.Get_dir()*len/(1 + rmass1/rmass2), fric, start_orient),
front (rmass2, r2, coor + start_orient.Get_dir()*len/(1 + rmass2/rmass1), fric, start_orient)
{
	if (health_ > max_health) max_health = health_;
	assert(Ok());
}
//--------------------------------------------------------------------------------------------------
void Car::Actions (float dt)
{
	assert(Ok());
	front.Applay_motion (dt);
	back.Applay_motion (dt);

	Orient ori(front.pos - back.pos);

//	float fm = 1/front.rev_mass, bm = 1/back.rev_mass;

	pos = (back.pos/back.rev_mass + front.pos/front.rev_mass)*rmass;
	front.pos = pos + ori.Get_dir()*lenght/(1 + back.rev_mass/front.rev_mass);
	back.pos = pos - ori.Get_dir()*lenght/(1 + front.rev_mass/back.rev_mass);

	back.orient = ori;
	Process_gestures (dt);
	Normalise_front_orient();

	Rudder_correction (dt);

}
Canvas* dbgcanv = 0;//deprecated;
bool collis_brd = true;
//--------------------------------------------------------------------------------------------------
void DBG_switch() { collis_brd = ! collis_brd;}
//--------------------------------------------------------------------------------------------------
void Car::Draw (Canvas* c)
{
	if (pic != 0)
		pic->draw (c, pos.To<int>(), (PI + back.orient.Get_angle())/PI/2);
	else	//if car hasn't sprite, it will be drawn schematically
	{
		dbgcanv = c;//deprecated;
		assert(Ok());
		Point begin = back.pos.To<int>();
		Point end = front.pos.To<int>();//(front.orient.Get_dir()*front.r + pos).To<int> ();

		Point leftback = Point (back.r*back.orient.Get_dir().y, -back.r*back.orient.Get_dir().x) + begin;
		Point leftfront = Point (front.r*back.orient.Get_dir().y, -front.r*back.orient.Get_dir().x) + end;
		Point rightback = Point (-back.r*back.orient.Get_dir().y, back.r*back.orient.Get_dir().x) + begin;
		Point rightfront = Point (-front.r*back.orient.Get_dir().y, front.r*back.orient.Get_dir().x) + end;

		Point one_trg = (front.r*front.orient.Get_dir() + front.pos).To<int>();
		Point two_trg = (front.r*(front.orient + Orient(2*PI/3)).Get_dir() + front.pos).To<int>();
		Point three_trg = (front.r*(front.orient - Orient(2*PI/3)).Get_dir() + front.pos).To<int>();

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
	if (!Dead())
	{
		Color headc (200, 150, 150);
		int hlen = health/max_health*Health_indicator_len;
		Rect hline (pos.x - hlen/2, pos.y + Health_indicator_offset, hlen, Health_indicator_height);
		hline.Draw (c, headc);
	}
}
//--------------------------------------------------------------------------------------------------
void Car::Collis_obj (Active* that)
{
    if (that->My_type() == Car_type)
    {
        Car* op = static_cast<Car*> (that);

//        Vector2f one[4];
		Shape one;
        Get_my_verticies (one.v);
		Shape two;
//        Vector2f two[4];
        op->Get_my_verticies (two.v);
        Collision_vector cv = Detect_collision (one, two, 0);//Collis_rectangles (one.v, two.v, false);
//		if (!cv.Vital()) cv = Detect_collision (one, two, 0);
        if (cv.Vital())//There is an collision
            Applay_obj_collision (op, cv);
    }
}
//--------------------------------------------------------------------------------------------------
void Car::Collis_brd (Rect with, float fric)
{
	if (!collis_brd) return;
	Vector2f lu (with.x, with.y);//left-up
	Vector2f ru (with.x + with.w, with.y);//right-up
	Vector2f ld (with.x, with.y + with.h);//left-down
	Vector2f rd (with.x + with.w, with.y + with.h);//right-down

	Vector2f one[4];
    Get_my_verticies (one);
	Vector2f two[4] = {lu, ru, rd, ld};

	Collision_vector rez;
	Collision_vector curv = Collis_rectangles (one, two, false);
	rez.papp = curv.papp;
	rez.depth = curv.depth;

	int num_iters = 1;

	while (num_iters-- > 0 && curv.Vital())
	{
		Move (curv.depth);

        Get_my_verticies (one);
		curv = Collis_rectangles (one, two, false);
		rez.papp = (rez.papp*rez.Weight() + curv.papp*curv.Weight())/(rez.Weight() + curv.Weight()); //averaging point of application
        rez.depth += curv.depth;
	}

    if (rez.Vital()) Applay_brd_collision (rez, fric);
}
//--------------------------------------------------------------------------------------------------
void Car::Drive_sand (Rect with, float fric)
{
	if (!collis_brd) return;
	Vector2f lu (with.x, with.y);//left-up
	Vector2f ru (with.x + with.w, with.y);//right-up
	Vector2f ld (with.x, with.y + with.h);//left-down
	Vector2f rd (with.x + with.w, with.y + with.h);//right-down

	Vector2f one[4];
    Get_my_verticies (one);
	Vector2f two[4] = {lu, ru, rd, ld};

	Shape myf;
	Get_my_front_verticies (myf.v);
	myf.Update_orthos ();

	Shape myb;
	Get_my_back_verticies (myb.v);
	myb.Update_orthos ();

	Shape sand;
	sand.v[0] = lu;
	sand.v[1] = ru;
	sand.v[2] = rd;
	sand.v[3] = ld;
	sand.Update_orthos ();

	Collision_vector rez  = Detect_collision (myb, sand, 0);
    if (rez.Vital()) Applay_sand_friction (rez, fric, Back_rect);

	rez  = Detect_collision (myf, sand, 0);
    if (rez.Vital()) Applay_sand_friction (rez, fric, Front_rect);
}
//--------------------------------------------------------------------------------------------------
void Car::Get_my_verticies (Vector2f* four)
{
	four[0] = Vector2f (back.r*back.orient.Get_dir().y, -back.r*back.orient.Get_dir().x) + back.pos;
	four[1] = Vector2f (-back.r*back.orient.Get_dir().y, back.r*back.orient.Get_dir().x) + back.pos;
	four[2] = Vector2f (-front.r*back.orient.Get_dir().y, front.r*back.orient.Get_dir().x) + front.pos;
	four[3] = Vector2f (front.r*back.orient.Get_dir().y, -front.r*back.orient.Get_dir().x) + front.pos;
}
//--------------------------------------------------------------------------------------------------
void Car::Get_my_front_verticies (Vector2f* four)
{
	four[0] = Vector2f (front.r*back.orient.Get_dir().y, -front.r*back.orient.Get_dir().x) + (back.pos + front.pos)/2;
	four[1] = Vector2f (-front.r*back.orient.Get_dir().y, front.r*back.orient.Get_dir().x) + (back.pos + front.pos)/2;
	four[2] = four[0] + (front.pos - back.pos)/2;
	four[3] = four[1] + (front.pos - back.pos)/2;
}
//--------------------------------------------------------------------------------------------------
void Car::Get_my_back_verticies (Vector2f* four)
{
	four[2] = Vector2f (back.r*back.orient.Get_dir().y, -back.r*back.orient.Get_dir().x) + (back.pos + front.pos)/2;
	four[3] = Vector2f (-back.r*back.orient.Get_dir().y, back.r*back.orient.Get_dir().x) + (back.pos + front.pos)/2;
	four[0] = four[2] - (front.pos - back.pos)/2;
	four[1] = four[3] - (front.pos - back.pos)/2;
}
//--------------------------------------------------------------------------------------------------
inline Vector2f From_point_to_stline (Vector2f end, Vector2f delta, Vector2f p)
{
	return (delta^(p-end))*delta/delta.Lensq() - (p - end);
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
			if (cur.Lensq() < ret.Lensq()) ret = cur;
		}
	return ret;
}
//-------------
inline bool On_line (const Vector2f& begin, const Vector2f& end, const Vector2f p)
{
	float delta = (p-begin).Proj (end - begin).Lensq() - (p-begin).Lensq();
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
			if (depth.Lensq() > ret.depth.Lensq())
			{
				ret.depth = depth;
				ret.papp = one[i];
			}
		}

	if (!recalled)
		for (int j = 0; j < 4; ++j)
			if (IN(one[0], one[1], one[2], one[3], cent1, two[j]))
				if (Get_depth (one, cent1, two[j], cent2, to_centras).Lensq() > ret.depth.Lensq())
				{
					ret = Collis_rectangles (two, one, to_centras, true);
					ret.depth = -ret.depth;
				}
	if (!ret.Vital ())
	{
		//!!! Needed to avoie a combining two rects same size
//		if (On_segment (one[0], one[1], two[])) //tratata
	}
	return ret;
}
#undef IN
//--------------------------------------------------------------------------------------------------
void Car::Applay_brd_collision (Collision_vector cv, float fric)
{
	if (Dead()) return;
    Vector2f imp_along =  Get_imp (cv.papp).Proj (cv.depth);

    if ((imp_along^cv.depth) < 0)
	{
        Appl_impulse (imp_along*(-1 - bouncy), cv.papp, 0.1);

		Vector2f vell = Get_vel (cv.papp);
		Vector2f vel_across = vell - vell.Proj (cv.depth);

		Appl_force (-fric*vel_across*cv.depth.Lensq (), cv.papp, true);
		if (Dead() && host != 0)
			host->Sub_frag ();
	}
}
//--------------------------------------------------------------------------------------------------
void Car::Applay_obj_collision (Car* with, Collision_vector cv)
{
	if (Dead()) return;
	if (with->Dead ()) return;
	float mass_sum = 1/rmass + 1/with->rmass;
	float rez_bouncy = bouncy*with->bouncy;
	assert (mass_sum > 0);
	Vector2f mydisp = cv.depth/rmass/mass_sum;
	Vector2f hisdisp = mydisp - cv.depth;
	
	with->Move (hisdisp);
	Move (mydisp);

	Vector2f my_imp = Get_imp (cv.papp).Proj (cv.depth);
	Vector2f his_imp = with->Get_imp (cv.papp).Proj (cv.depth);
	Vector2f along_dimp = (my_imp - his_imp);

	if ((along_dimp^cv.depth) < 0)
	{
		Vector2f ci_vel = (my_imp + his_imp)/mass_sum;//inertia center velocity

		Vector2f mydimp = (1 + rez_bouncy)*(ci_vel/rmass - my_imp);
		Vector2f hisdimp = (1 + rez_bouncy)*(ci_vel/with->rmass - his_imp);
		Appl_impulse (mydimp, cv.papp);
		with->Appl_impulse (hisdimp, cv.papp);

		if (with->Dead() && host != 0)
			if (with->host == host) host->Sub_frag ();
			else					host->Add_frag ();
		if (Dead() && with->host != 0)						//frag encouting
			if (with->host == host) with->host->Sub_frag ();
			else					with->host->Add_frag ();
	}
}
//--------------------------------------------------------------------------------------------------
void Car::Applay_sand_friction (Collision_vector cv, float fric, bool bORf)
{
	if (Dead()) return;
    Vector2f imp = Get_imp (cv.papp);
	Vector2f resist;

	if (bORf == Back_rect)
		resist = back.Get_resist (Get_vel (cv.papp))*(cv.depth.Lensq ()/back.r/back.r);
	else
		resist = front.Get_resist (Get_vel (cv.papp))*(cv.depth.Lensq ()/front.r/front.r);

	resist *= fric;

	Appl_force (resist, cv.papp, true);
	if (Dead() && host != 0)
		host->Sub_frag ();
}
//--------------------------------------------------------------------------------------------------
Vector2f Car::Get_vel (Vector2f p)
{
    Vector2f trans = back.vel;

    Vector2f axis = (front.pos - back.pos)/lenght;//normalize
    Vector2f dir = p - back.pos;
    float distance = dir.Len();
    dir /= distance;                            //normalize

    Orient alfa = Orient (dir, true) - Orient (axis, true);

    Vector2f rotary = alfa.Rotate (front.vel - back.vel)*distance/lenght;

    return trans + rotary;
}
//--------------------------------------------------------------------------------------------------
Vector2f Car::Get_imp (Vector2f p)
{
    Vector2f to_back = p - back.pos;
    Vector2f to_front = p - front.pos;

    Vector2f back_imp = back.Get_impulse().Proj (to_back);
    Vector2f front_imp = front.Get_impulse().Proj (to_front);

	return back_imp + front_imp;
}
//--------------------------------------------------------------------------------------------------
bool Car::Damage (Vector2f imp, Vector2f papp, float destructive_k)
{
	float val = imp.Lensq ()*destructive_k;//!!! too plain formula

	health -= val;
	if (Dead())
	{
		if (host) host->Car_crashed ();
		host = 0;
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------
bool Car::Dead() const
{
	return health <= 0;
}
//--------------------------------------------------------------------------------------------------
void Car::Appl_impulse (Vector2f imp, Vector2f papp, float destructive_k)
{
    Vector2f to_back = papp - back.pos;
    Vector2f to_front = papp - front.pos;
    float det = to_back.x*to_front.y - to_back.y*to_front.x;

    if (-aboutnull < det && det < aboutnull) //impulse along the cardan shaft
    {
		Vector2f dvel = imp*rmass;
		back.Appl_impulse (dvel/back.rev_mass);
		front.Appl_impulse (dvel/front.rev_mass);
        return;
    }

    Vector2f back_proj = to_back * (imp.x*to_front.y - imp.y*to_front.x)/det;
    Vector2f front_proj = to_front * (imp.y*to_back.x - imp.x*to_back.y)/det;// oblique projections

    Vector2f axis = front.pos - back.pos;
    Vector2f axis_bproj = (back_proj^axis)*axis/axis.Lensq();
    Vector2f axis_fproj = (front_proj^axis)*axis/axis.Lensq();

    Vector2f ortho_bproj = back_proj - axis_bproj;//this part takes only front
    Vector2f ortho_fproj = front_proj - axis_fproj;//this part takes only back


    Vector2f dvel_axis = (axis_bproj + axis_fproj)*rmass;//this part of impulse take both front and back

    back.Appl_impulse (dvel_axis/back.rev_mass + ortho_bproj);
    front.Appl_impulse (dvel_axis/front.rev_mass + ortho_fproj);

	Damage (imp, papp, destructive_k);
}
//--------------------------------------------------------------------------------------------------
void Car::Appl_force (Vector2f f, Vector2f papp, bool resistancive)
{
    Vector2f to_back = papp - back.pos;
    Vector2f to_front = papp - front.pos;
    float det = to_back.x*to_front.y - to_back.y*to_front.x;

    if (-aboutnull < det && det < aboutnull) //impulse along the cardan shaft
    {
		Vector2f acc = f*rmass;
		back.Appl_force (acc/back.rev_mass, resistancive);
		front.Appl_force (acc/front.rev_mass, resistancive);
        return;
    }

    Vector2f back_proj = to_back * (f.x*to_front.y - f.y*to_front.x)/det;
    Vector2f front_proj = to_front * (f.y*to_back.x - f.x*to_back.y)/det;// oblique projections

    Vector2f axis = front.pos - back.pos;
    Vector2f axis_bproj = (back_proj^axis)*axis/axis.Lensq();
    Vector2f axis_fproj = (front_proj^axis)*axis/axis.Lensq();

    Vector2f ortho_bproj = back_proj - axis_bproj;//this part takes only front
    Vector2f ortho_fproj = front_proj - axis_fproj;//this part takes only back


    Vector2f acc_axis = (axis_bproj + axis_fproj)*rmass;//this part of impulse take both front and back

    back.Appl_force (acc_axis/back.rev_mass + ortho_bproj, resistancive);
    front.Appl_force (acc_axis/front.rev_mass + ortho_fproj, resistancive);
}
//--------------------------------------------------------------------------------------------------
void Car::Process_gestures (float dt)
{
	assert(Ok());
	if (fp && !bp) Motory_force ( motor_force);
	if (bp && !fp) Motory_force (-motor_force);
	if (rp && !lp) Turn_front ( angular_vel*dt);
	if (lp && !rp) Turn_front (-angular_vel*dt);
}
//--------------------------------------------------------------------------------------------------
void Car::Motory_force (float f)
{
	assert(Ok());
	Vector2f accl = f*rmass*back.orient.Get_dir ();
	back.Appl_force (accl/back.rev_mass);
	front.Appl_force (accl/front.rev_mass);
}
//--------------------------------------------------------------------------------------------------
void Car::Turn_front (float dfi)
{
	assert(Ok());
//	Orient delta = front.orient - back.orient;
//	if (delta + dfi > Max_ang_dev) dfi = Max_ang_dev - delta;
//	if (delta + dfi < -Max_ang_dev) dfi = -Max_ang_dev - delta;

	front.orient += dfi;
}
//--------------------------------------------------------------------------------------------------
void Car::Normalise_front_orient()
{
	assert(Ok());
	float delta = In_range ((front.orient - back.orient).Get_angle ());
	if (delta > Max_ang_dev) front.orient += Max_ang_dev - delta;
	if (delta < -Max_ang_dev) front.orient += -Max_ang_dev - delta;
}
//--------------------------------------------------------------------------------------------------
void Car::Rudder_correction (float dt)
{
	assert(Ok());
	float delta = In_range ((front.orient - back.orient).Get_angle ());
	front.orient = back.orient + delta*(1 - rudder_spring*dt);
}
//--------------------------------------------------------------------------------------------------
inline bool Boolly (bool arg) {return arg == true || arg == false;}
bool Car::Ok() const
{
	return Boolly (fp) && Boolly (bp) && Boolly (lp) && Boolly (rp) &&
		back.Ok () && front.Ok () && lenght > 0 && rmass > 0;
}
//--------------------------------------------------------------------------------------------------

