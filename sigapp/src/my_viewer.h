# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>

# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>

// Viewer for this example:
class MyViewer : public WsViewer
{  protected :
	enum MenuEv { EvNormals, EvAnimate, EvExit };
	UiCheckButton* _nbut;
	bool _animating;

   public :
	//Maipulators for heaad, torso, arms and legs
	MyViewer ( int x, int y, int w, int h, const char* l );
	void build_ui ();
	void add_model ( SnShape* s, GsVec p );
	void build_scene ();
	virtual int handle_keyboard ( const GsEvent &e ) override;
	virtual int uievent ( int e ) override;
	void animate();

	void moveNPC(float a, float b, float c);
	double moveCurves(float x, float y, float z, double dist);
	void moveChar( float a, float b, float c);
	bool checkCollision(float x, float y, float z, float dir);
	void animateCoral(float a);
	GsMat computeShadow();

	

};

