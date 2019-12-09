# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

double px = 0.0;
double py = 0.0;
double pz = 0.0;
double x, y, z = 0.0;
double npcC[3][10]; // coordinates, X,Y,Z, i=0->9
float degrees = (float)GS_PI / 180;
GsVec lightPos = GsVec(0,-500,0);

//Default scale of the objs
float scale = 40.0;

//These variables keep track of where the joints are
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

	//This is how we make shadow
	//How we achieved is by flattening the copy of the original obj
	//ComputeShadow() gives us position where the shadow might be in a matrix
	//After that, the process is same as a normal objects in this code
	//So if we make an object and add it using this function...
	//Its gonna make that object, and its shadow... So two objects

	//If u want to edit the first object, you would call it like this...
	//	SnManipulator* firstObj = rootg()->get<SnManipulator>(0);  GsMat f = firstObj->mat();
	//	and apply transformation and whaat not
	// If u want to edit the second object that you added, u would have to call...
	//SnManipulator* secondObj = rootg()->get<SnManipulator>(2); 

	//2 instead of 1 because, rootg object at 1 is the shadow of the first object
	//and rootg at 3 contains shadow of the second object.. and so on....



	/*SnManipulator* shadow = new SnManipulator;
	GsMat shad = computeShadow();
	shad.rcombtrans(p);
	shadow->initial_mat(shad);*/


	SnGroup* g = new SnGroup;
	SnLines* l = new SnLines;
	l->color(GsColor::orange);
	g->add(s);
	g->add(l);
	manip->child(g);
	//shadow->child(g); //child for shadow
	//shadow->visible(false); //No mouse interaction for shadow
	manip->visible(true); // call this to turn off mouse interaction
	rootg()->add(manip);
	//rootg()->add(shadow); //adding shadow to the root
	

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
		// This is not relevant
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
	}
	//New Code

	// the fish that is controled by player
	SnModel* playerFish = new SnModel;
	playerFish->model()->load_obj("../reefObjs/PlayerFish/blackMoorFish.obj");
	GsModel* pf = playerFish->model();
	pf->scale(scale);
	add_model(playerFish, GsVec(px, py, pz));

	//Angel Fishes
	// sets variables quickly because it needs to be done
	for (int q = 0; q < 3; q++) {
		for (int i = 0; i < 10; i ++){
			npcC[q][i] = (q * i * 100);
		}
	}
	// makes the fishes
	SnModel* af[10]; // the NPC fishes
	GsModel* gsaf[10]; // NPC Fishes
	for (int i = 0; i < 10; i++) {
		af[i] = new SnModel;
		af[i]->model()->load_obj("../reefObjs/angelFish/angelFish.obj");
		gsaf[i] = af[i]->model();
		gsaf[i]->scale(scale);
		add_model(af[i], GsVec(npcC[0][i], npcC[1][i], npcC[2][i]));
	}

	//Corals and such
	// TableCoral/ Tree Corals = tc
	SnModel* tc1 = new SnModel;
	tc1->model()->load_obj("../reefObjs/TableCoral1/tableCoral1.obj");
	GsModel* gstc1 = tc1->model();
	gstc1->translate(GsVec(-10, -10, 0));
	gstc1->scale(scale);
	add_model(tc1, GsVec(x, y, z));

	// TreeCoral
	SnModel* tc2 = new SnModel;
	tc2->model()->load_obj("../reefObjs/TreeCoral1/treeCoral1.obj");
	GsModel* gstc2 = tc2->model();
	gstc2->translate(GsVec(-10, -10, 0));
	gstc2->scale(scale);
	add_model(tc2, GsVec(x, y, z));

	// Spiral Wire Coral
	SnModel* swc = new SnModel;
	swc->model()->load_obj("../reefObjs/spiralWireCoral/spiralWireCoral.obj");
	GsModel* gsswc = swc->model();
	gsswc->translate(GsVec(-10, -10, 0));
	gsswc->scale(scale);
	add_model(swc, GsVec(x, y, z));

	//Soft Coral
	SnModel* sc = new SnModel;
	sc->model()->load_obj("../reefObjs/softCoral/softCoral.obj");
	GsModel* gssc = sc->model();
	gssc->translate(GsVec(-10, -10, 0));
	gssc->scale(scale);
	add_model(sc, GsVec(x, y, z));

	// PillarCoral
	SnModel* pc = new SnModel;
	pc->model()->load_obj("../reefObjs/pillarCoral/pillarCoral.obj");
	GsModel* gspc = pc->model();
	gspc->translate(GsVec(-10, -10, 0));
	gspc->scale(scale);
	add_model(pc, GsVec(x, y, z));

}


//void MyViewer::moveleftarm(float xx)
//{
//	SnManipulator* larm = rootg()->get<SnManipulator>(11);
//	GsMat armMat = larm->mat();
//	GsMat tr;
//	leftarmcntr += xx;
//
//	tr.translation(GsVec(x, y, z));
//	armMat.rotz(leftarmcntr);
//	armMat.mult(tr, armMat);
//	larm->initial_mat(armMat);
//	render();
//	ws_check();
//}

//void MyViewer::movehead(float xx)
//{
//	SnManipulator* larm = rootg()->get<SnManipulator>(7);
//	GsMat armMat = larm->mat();
//	GsMat tr;
//	GsMat rot;
//	headcntr += xx;
//	rot.rotz(headcntr);
//	tr.translation(GsVec(0, 0, 0));
//
//	armMat.rotx(headcntr);
//	armMat.mult(tr, armMat);
//	larm->initial_mat(armMat);
//	render();
//	ws_check();
//}

//void MyViewer::moveall(float a, float b, float c)
//{
//	x += a;
//	y += b;
//	z += c;
//	  
//	SnManipulator* rleg = rootg()->get<SnManipulator>(1);
//	GsMat rlMat = rleg->mat();
//
//	SnManipulator* lleg= rootg()->get<SnManipulator>(3);
//	GsMat llMat = lleg->mat();
//
//	SnManipulator* torso = rootg()->get<SnManipulator>(5);
//	GsMat tMat = torso->mat();
//
//	SnManipulator* head = rootg()->get<SnManipulator>(7);
//	GsMat hMat = head->mat();
//
//	SnManipulator* rarm = rootg()->get<SnManipulator>(9);
//	GsMat raMat = rarm->mat();
//
//	SnManipulator* larm = rootg()->get<SnManipulator>(11);
//	GsMat laMat =larm->mat();
//
//
//	rlMat.translation(GsVec(x, y, z));
//	llMat.translation(GsVec(x, y, z));
//	tMat.translation(GsVec(x, y, z));
//	hMat.translation(GsVec(x, y, z));
//	raMat.translation(GsVec(x, y, z));
//	laMat.translation(GsVec(x, y, z));
//
//	rleg->initial_mat(rlMat);
//	lleg->initial_mat(llMat);
//	torso->initial_mat(tMat);
//	head->initial_mat(hMat);
//	rarm->initial_mat(raMat);
//	larm->initial_mat(laMat);
//
//	render();
//	ws_check();
//}


int MyViewer::handle_keyboard(const GsEvent& e)
{
	int ret = WsViewer::handle_keyboard(e); // 1st let system check events
	if (ret) return ret;

	switch (e.key)
	{
		case GsEvent::KeyEsc: gs_exit(); return 1;
		default: gsout << "Key pressed: " << e.key << gsnl;

		case 'w':{ // +X
			px += 0.1;
			return 1;
		}
		case 's':{ // -X
			px -= 0.1;
			return 1;
		}
		case 'd':{ // +Y
			py += 0.1;
			return 1;
		}
		case 'a': { // -Y
			py -= 0.1;
			return 1;
		}
		case GsEvent::KeySpace:{ // +Z
			pz += 0.1;
			return 1;
		}
		case GsEvent::KeyShift: { // -Z
			pz -= 0.1;
			return 1;
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