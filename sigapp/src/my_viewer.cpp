# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

//TO DO LIST
/*
1. Animation on Reef Ojbects
2. NPC Fish follow curves
3. NON trivial Animation on ALL fishes
4. Automated Movements for NPC Fishes
5. Collision Detection
6. Add Aquarium Object (Hollow, but exists for boundaries)
*/


double px = 0.0;
double py = 0.0;
double pz = 0.0;
double x, y, z = 0.0;
double npcC[3][4]; // coordinates, X,Y,Z, i=0->3 First is Coord (x,y,z), Second is the Fish
float degrees = (float)GS_PI / 180;

float dir = 0;
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

	SnManipulator* shadow = new SnManipulator;
	GsMat shad = computeShadow();
	shad.rcombtrans(p);
	shadow->initial_mat(shad);

	SnGroup* g = new SnGroup;
	SnLines* l = new SnLines;
	l->color(GsColor::orange);
	g->add(s);
	g->add(l);
	manip->child(g);
	shadow->child(g); //child for shadow
	shadow->visible(false); //No mouse interaction for shadow
	manip->visible(true); // call this to turn off mouse interaction
	rootg()->add(manip);
	rootg()->add(shadow); //adding shadow to the root

}

GsMat MyViewer::computeShadow()
{
	float lx = 5.0f;
	float ly = 10.0f;
	float lz = 15.0f;
	GsMat s = GsMat(1.0f, (-lx / ly), 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, (-lz / ly), 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	GsMat tr;
	tr.translation(GsVec(-200, -350, -800));

	GsMat d = tr * s;
	return d;
}

void MyViewer::moveNPC(float a, float b, float c) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 10; j++) {
			if (i == 0) {
				npcC[i][j] += a;
			}
			if (i == 1) {
				npcC[i][j] += b;
			}
			if (i == 2) {
				npcC[i][j] += c;
			}
		}
	}

	const int npcNum = 4;
	SnManipulator* npF[npcNum];
	GsMat npFmat[npcNum];
	// MOVES TOP ONE 10 TIMES
	for (int i = 0; i < npcNum; i++) {
		npF[i] = rootg()->get<SnManipulator>(i + 1);
		npFmat[i] = npF[i]->mat();
		npFmat[i].translation(GsVec(npcC[0][i], npcC[1][i], npcC[2][i]));
		npF[i]->initial_mat(npFmat[i]);
		npF[i]->translation(GsVec(a, b, c));
		render();
		ws_check();
	}
}

void MyViewer::build_scene()
{
	rootg()->remove_all();

	// the fish that is controled by player
	SnModel* playerFish = new SnModel;
	playerFish->model()->load_obj("../reefObjs/PlayerFish/blackMoorFish.obj");
	GsModel* pf = playerFish->model();
	pf->translate(GsVec(5, 5, 5));
	pf->scale(scale);
	add_model(playerFish, GsVec(px, py, pz));

	//Angel Fishes
	// sets variables quickly because it needs to be done
	//Fish 0
	npcC[0][0] = 250;
	npcC[1][0] = 250;
	npcC[2][0] = 250;
	//Fish 1
	npcC[0][1] = -250;
	npcC[1][1] = 250;
	npcC[2][1] = 250;
	//Fish 2
	npcC[0][2] = -250;
	npcC[1][2] = 250;
	npcC[2][2] = -250;
	//Fish 3
	npcC[0][3] = 250;
	npcC[1][3] = 250;
	npcC[2][3] = -250;

	// makes the fishes
	const int npcNum = 4;
	SnModel* af[npcNum]; // the NPC fishes
	GsModel* gsaf[npcNum]; // NPC Fishes
	for (int i = 0; i < npcNum; i++) {
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
	gstc1->translate(GsVec(-5, -10, 0));
	gstc1->scale(scale);
	add_model(tc1, GsVec(x, y, z));

	//Soft Coral
	SnModel* sc = new SnModel;
	sc->model()->load_obj("../reefObjs/softCoral/softCoral.obj");
	GsModel* gssc = sc->model();
	gssc->translate(GsVec(-10, -10, -10));
	gssc->scale(scale);
	add_model(sc, GsVec(x, y, z));

	// TreeCoral
	SnModel * tc2 = new SnModel;
	tc2->model()->load_obj("../reefObjs/TreeCoral1/treeCoral1.obj");
	GsModel * gstc2 = tc2->model();
	gstc2->translate(GsVec(-15, -10, 0));
	gstc2->scale(scale);
	add_model(tc2, GsVec(x, y, z));

	// Spiral Wire Coral
	SnModel * swc = new SnModel;
	swc->model()->load_obj("../reefObjs/spiralWireCoral/spiralWireCoral.obj");
	GsModel * gsswc = swc->model();
	gsswc->translate(GsVec(13, -10, -2));
	gsswc->scale(scale);
	add_model(swc, GsVec(x, y, z));

	// PillarCoral
	SnModel * pc = new SnModel;
	pc->model()->load_obj("../reefObjs/pillarCoral/pillarCoral.obj");
	GsModel * gspc = pc->model();
	gspc->translate(GsVec(15, -10, 5));
	gspc->scale(scale);
	add_model(pc, GsVec(x, y, z));

	// BoxBox
	SnPrimitive * p;
	p = new SnPrimitive(GsPrimitive::Box, 45, 0.5, 45);
	p->prim().material.diffuse = GsColor::lightblue;
	GsModel * d = p->model();
	d->translate(GsVec(0, -400, 0));
	d->scale(scale);
	rootg()->add(p);

	SnPrimitive * w;
	w = new SnPrimitive(GsPrimitive::Box, 45, 18, 0.5);
	w->prim().material.diffuse = GsColor::yellow;
	GsModel * x = w->model();
	x->translate(GsVec(0, 300, -1800));
	x->scale(scale);
	rootg()->add(w);
}

void MyViewer::moveChar(float a, float b, float c)
{
	px += a;
	py += b;
	pz += c;
	SnManipulator* player = rootg()->get<SnManipulator>(0);
	GsMat pMat = player->mat();
	pMat.translation(GsVec(px, py, pz));
	player->initial_mat(pMat);

	SnManipulator* shadow = rootg()->get<SnManipulator>(1);
	GsMat sMat = player->mat();
	//sMat.translation();
	sMat.rcombtrans(GsVec(x, y, z));
	shadow->initial_mat(sMat);
	player->translation(GsVec(a, b, c));
	render();
	ws_check();
}

int MyViewer::handle_keyboard(const GsEvent & e)
{
	int ret = WsViewer::handle_keyboard(e); // 1st let system check events
	if (ret) return ret;

	switch (e.key)
	{
	case GsEvent::KeyEsc: gs_exit(); return 1;
	default: gsout << "Key pressed: " << e.key << gsnl;

	case 'q': { // +X
		if (px < 1000) {
			moveChar(5, 0, 0);
		}
		return 1;
	}
	case 'a': { // -X
		if (px > -1000) {
			moveChar(-5, 0, 0);
		}
		return 1;
	}
	case 'w': { // +Y
		if (py < 1000) {
			moveChar(0, 5, 0);
		}
		return 1;
	}
	case 's': { // -Y
		if (py > -1000) {
			moveChar(0, -5, 0);
		}
		return 1;
	}
	case 'e': { // +Z
		if (pz < 1000) {
			moveChar(0, 0, 5);
		}
		return 1;
	}
	case 'd': { // -Z
		if (pz > -1000) {
			moveChar(0, 0, -5);
		}
		return 1;
	}
	case 'r': { // MOVE NPCs
		moveNPC(1, 1, 1);
		return 1;
	}
	case 'f': { // MOVE NPCs
		moveNPC(-1, -1, -1);
		return 1;
	}
	case GsEvent::KeyLeft: {
		return 1;
	}
	case GsEvent::KeyUp: {
		return 1;
	}
	case GsEvent::KeyRight: {
		return 1;
	}
	case GsEvent::KeyDown: {
		return 1;
	}
	case GsEvent::KeySpace:
	{
		//Moving camera up, could be improved to rotate around
		/*double lt, t0 = gs_time();
		do
		{
			lt = gs_time() - t0;

			camera().eye.y += 2.5f;
			camera().eye.z -= 3.0f;
			camera().up.z += 0.001f;
			render();
			ws_check();
			message().setf("local time=%f", lt);
		} while (lt < 1.5f);*/
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