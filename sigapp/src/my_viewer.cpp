# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

double x = 0.0;
double y = 0.0;
double z = 0.0;
float degrees = (float)GS_PI / 180;
GsVec lightPos = GsVec(0,-500,0);


//These variables keep track of where the joints are
float leftarmcntr = 0;
float rightarmcntr = 0; // Freancesoc ws here
float leftlegcntr = 0;
float rightlegcntr = 0;
float headcntr = 0;

float pi = 3.14f;
float af = pi / 100;

MyViewer::MyViewer(int x, int y, int w, int h, const char* l) : WsViewer(x, y, w, h, l)
{
	_nbut = 0;
	_animating = false;
	build_ui();
	build_scene();

}

void MyViewer::build_ui()
{
	UiPanel* p;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel("", UiPanel::HorizLeft);
	p->add(new UiButton("Animate", EvAnimate));
	p->add(new UiButton("Exit", EvExit)); p->top()->separate();
}

void MyViewer::add_model(SnShape* s, GsVec p)
{
	SnManipulator* manip = new SnManipulator;
	GsMat m;
	m.translation(p);
	manip->initial_mat(m);

	SnManipulator* shadow = new SnManipulator;
	GsMat shad = computeShadow();
	shad.rcombtrans(p);
	//shad.translation(lightPos);
	shadow->initial_mat(shad);
	SnGroup* g = new SnGroup;
	SnLines* l = new SnLines;
	l->color(GsColor::orange);
	g->add(s);
	g->add(l);
	manip->child(g);
	shadow->child(g);
	shadow->visible(false);
	manip->visible(false); // call this to turn off mouse interaction
	rootg()->add(manip);
	rootg()->add(shadow);
	

}
GsMat MyViewer::computeShadow()
{
	GsLight l;
	//float lx = l.position.x;
	//float ly = l.position.y;
	//float lz = l.position.z;
	float lx = 5.0f;
	float ly = 10.0f;
	float lz = 15.0f;
	GsMat s = GsMat(1.0f, (-lx / ly), 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, (-lz / ly), 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	GsMat tr;
	tr.translation(GsVec(-200, -600, -800));

	GsMat d = tr * s;
	return d;
}
void MyViewer::build_scene()
{
	rootg()->remove_all();
	{
		////Floor
		//SnPrimitive* p;
		//p = new SnPrimitive(GsPrimitive::Box, 45, 0.5, 45);
		//p->prim().material.diffuse = GsColor::yellow;
		//GsModel* d = p->model();
		//d->translate(GsVec(0, -700, 0));
		//d->scale(35);
		//rootg()->add(p);

		////Right Leg
		////load the obj to the model
		////rotate the matrices and transform to change the alignment
		////Translate to the proper place
		////scale it to change the size
		//SnModel* leg = new SnModel;
		//leg->model()->load_obj("../objs/leg.obj");
		//GsModel* o = leg->model();
		//GsMat lmatz;
		//lmatz.rotx(-100 * degrees);
		//o->transform(lmatz, false);
		//o->translate(GsVec(-15, -50, 0));
		//o->scale(13);
		//add_model(leg, GsVec(x, y, z));

		////Left Leg
		////Same step as left leg
		////I am mirroring the obj in x axis
		////Using Identity matrix mirr and multiplying it to input matrix lrmatz, then transforming it
		////The model appears black because I believe its turning it inside out, do not know how to fix it yet
		//SnModel* legr = new SnModel;
		//legr->model()->load_obj("../objs/leg.obj");
		//GsModel* lr = legr->model();
		//GsMat lrmatz;
		//GsMat mirr(-1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		//lrmatz.rotx(-100 * degrees);
		//GsMat mirrmat = lrmatz * mirr;
		//lr->transform(mirrmat, false);
		//lr->translate(GsVec(13, -50, 0));
		//lr->scale(13);
		//add_model(legr, GsVec(x, y, z));


		////Torso
		//SnModel* torso = new SnModel;
		//torso->model()->load_obj("../objs/torso.obj");
		//GsModel* t = torso->model();
		//GsMat tmat;
		//tmat.rotx(-100 * degrees);
		//t->transform(tmat, false);
		//t->translate(GsVec(0, 50, -20));
		//t->scale(18);
		//add_model(torso, GsVec(x, y, z));

		////Head
		//SnModel* head = new SnModel;
		//head->model()->load_obj("../objs/TheRock2.obj");
		//GsModel* h = head->model();
		//h->translate(GsVec(-15, 915, -210));
		//h->scale(2);
		//add_model(head, GsVec(x, y, z));

		////Right Arm
		//SnModel* rarm = new SnModel;
		//rarm->model()->load_obj("../objs/arm.obj");
		//GsModel* rm = rarm->model();
		//GsMat ramat;
		//ramat.rotx(-100 * degrees);
		//rm->transform(ramat, false);
		//rm->translate(GsVec(-40, 60, -20));
		//rm->scale(15);
		//add_model(rarm, GsVec(x, y, z));

		//////Left Arm
		//SnModel* larm = new SnModel;
		//larm->model()->load_obj("../objs/arm.obj");
		//GsModel* lm = larm->model();
		//GsMat lamat;
		////I am going to use the same GsMat mirr
		//lamat.rotx(-100 * degrees);
		//GsMat mirr2mat = lamat * mirr;
		//lm->transform(mirr2mat, false);
		//lm->translate(GsVec(35, 60, -20));
		//lm->scale(15);
		//add_model(larm, GsVec(x, y, z));

		////objs
		//////heli
		////SnModel* heli = new SnModel;
		////heli->model()->load_obj("../objs/Heli_bell.obj");
		////GsModel* he = heli->model();
		////he->translate(GsVec(-20,-20,25));
		////he->scale(35);
		////add_model(heli, GsVec(x, y, z));

		////beanbag
		//SnModel* bean = new SnModel;
		//bean->model()->load_obj("../objs/beanbag.obj");
		//GsModel* be = bean->model();
		//GsMat bb;
		//bb.rotx(-100 * degrees);
		//be->transform(bb, false);
		//be->translate(GsVec(60, -45, -40));
		//be->scale(15);
		//add_model(bean, GsVec(x, y, z));	

	}// This is not relevant

	SnModel* shell = new SnModel;
	shell->model()->load_obj("../reefObjs/seashell1/seashell_obj.obj");
	GsModel* sh = shell->model();

	sh->scale(10);
	add_model(shell, GsVec(x, y, z));

	SnModel* shell2 = new SnModel;
	shell2->model()->load_obj("../reefObjs/seashell1/seashell_obj.obj");
	GsModel* sh2 = shell2->model();

	sh2->scale(10);
	add_model(shell2, GsVec(x+10, y, z));
}


void MyViewer::moveleftarm(float xx)
{
	SnManipulator* larm = rootg()->get<SnManipulator>(11);
	GsMat armMat = larm->mat();
	GsMat tr;
	leftarmcntr += xx;

	tr.translation(GsVec(x, y, z));
	armMat.rotz(leftarmcntr);
	armMat.mult(tr, armMat);
	larm->initial_mat(armMat);
	render();
	ws_check();
}

void MyViewer::moverightarm(float xx)
{
	SnManipulator* larm = rootg()->get<SnManipulator>(9);
	GsMat armMat = larm->mat();
	rightarmcntr += xx;
	armMat.rotz(rightarmcntr);
	larm->initial_mat(armMat);
	render();
	ws_check();
}

void MyViewer::moveleftleg(float xx)
{
	SnManipulator* larm = rootg()->get<SnManipulator>(3);
	GsMat armMat = larm->mat();
	leftlegcntr += xx;
	armMat.rotx(leftlegcntr);
	larm->initial_mat(armMat);
	render();
	ws_check();
}

void MyViewer::moverightleg(float xx)
{
	SnManipulator* larm = rootg()->get<SnManipulator>(1);
	GsMat armMat = larm->mat();
	rightlegcntr += xx;
	armMat.rotx(rightlegcntr);
	larm->initial_mat(armMat);
	render();
	ws_check();
}

void MyViewer::movehead(float xx)
{
	SnManipulator* larm = rootg()->get<SnManipulator>(7);
	GsMat armMat = larm->mat();
	GsMat tr;
	GsMat rot;
	headcntr += xx;
	rot.rotz(headcntr);
	tr.translation(GsVec(0, 0, 0));

	armMat.rotx(headcntr);
	armMat.mult(tr, armMat);
	larm->initial_mat(armMat);
	render();
	ws_check();
}

void MyViewer::moveall(float a, float b, float c)
{
	x += a;
	y += b;
	z += c;
	  
	SnManipulator* rleg = rootg()->get<SnManipulator>(1);
	GsMat rlMat = rleg->mat();

	SnManipulator* lleg= rootg()->get<SnManipulator>(3);
	GsMat llMat = lleg->mat();

	SnManipulator* torso = rootg()->get<SnManipulator>(5);
	GsMat tMat = torso->mat();

	SnManipulator* head = rootg()->get<SnManipulator>(7);
	GsMat hMat = head->mat();

	SnManipulator* rarm = rootg()->get<SnManipulator>(9);
	GsMat raMat = rarm->mat();

	SnManipulator* larm = rootg()->get<SnManipulator>(11);
	GsMat laMat =larm->mat();


	rlMat.translation(GsVec(x, y, z));
	llMat.translation(GsVec(x, y, z));
	tMat.translation(GsVec(x, y, z));
	hMat.translation(GsVec(x, y, z));
	raMat.translation(GsVec(x, y, z));
	laMat.translation(GsVec(x, y, z));

	rleg->initial_mat(rlMat);
	lleg->initial_mat(llMat);
	torso->initial_mat(tMat);
	head->initial_mat(hMat);
	rarm->initial_mat(raMat);
	larm->initial_mat(laMat);

	render();
	ws_check();
}


int MyViewer::handle_keyboard(const GsEvent& e)
{
	int ret = WsViewer::handle_keyboard(e); // 1st let system check events
	if (ret) return ret;

	switch (e.key)
	{
	case GsEvent::KeyEsc: gs_exit(); return 1;
	default: gsout << "Key pressed: " << e.key << gsnl;

	case 'q':
	{
		if (rightarmcntr >= -0.098125f)
			moverightarm(-af);
		return 1;
	}
	case 'a':
	{
		if (rightarmcntr <= 0.049063f)
			moverightarm(af);
		return 1;
	}


	case 'w':
	{
		if (leftarmcntr >= -0.098125f)
			moveleftarm(-af);
		return 1;
	}

	case 's':
	{
		if (leftarmcntr <= 0.049063f)
			moveleftarm(af);
		message().setf("leftaramcntr=%f", leftarmcntr);
		return 1;
	}

	case 'e':
	{
		if (rightlegcntr >= -0.1)
			moverightleg(-af);
		return 1;
	}
	case 'd':
	{
		if (rightlegcntr <= 0.12)
			moverightleg(af);
		return 1;
	}
	case 'r':
	{
		if (leftlegcntr >= -0.1)
			moveleftleg(-af);
		return 1;
	}
	case 'f':
	{
		if (leftlegcntr <= 0.12)
			moveleftleg(af);
		
		return 1;
	}
	case 'z':
	{
		if (headcntr >= -0.049062f)
			movehead(-af);
		return 1;
	}
	case 'x':
	{
		if(headcntr<= 0.049063f)
			movehead(af);
		return 1;
	}

	case GsEvent::KeyLeft:
	{
		moveall(-3.0f, 0, 0);
		return 1;
	}
	case GsEvent::KeyUp:
	{
		moveall(0, 0, -3.0f);
		return 1;
	}

	case GsEvent::KeyRight:
	{
		moveall(3.0f, 0, 0);
		return 1;
	}

	case GsEvent::KeyDown:
	{
		moveall(0, 0, 3.0f);
		return 1;
	}

	case GsEvent::KeySpace:
	{
		double lt, t0 = gs_time();
		do
		{
			lt = gs_time() - t0;

			//camera().eye.x += 1.0f;
			camera().eye.y += 2.5f;
			camera().eye.z -= 3.0f;
			//camera().center.y += 1.0f;
			//camera().center.x += 1.0f;

			camera().up.z += 0.001f;
			render();
			ws_check();
			message().setf("local time=%f", lt);
		} while (lt < 1.5f);
	}
	}

	return 0;
}

int MyViewer::uievent(int e)
{
	switch (e)
	{
		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}