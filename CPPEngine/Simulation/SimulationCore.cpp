// SimulationCore.cpp: implementation of the SimulationCore class.
//
//////////////////////////////////////////////////////////////////////

#include "Simulation/SimulationCore.h"
#include "Camera/Camera.h"
#include "SimObject/Types/JackCube.h"
#include "Glide/GlideManager.h"
#include "SimObject/PhysicsSim.h"

#include "Hud/HudManager.h"
#include "Hud/Elements/HudText.h"
#include "Bitmap/BMP.h"
#include "Bitmap/BMFont.h"
#include "Render/RenderLaser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FxU32 texMinAddress;
FxU32 texMaxAddress;

Model3D *badguys[5];
GlideTexture *textureExample;

WavefrontObj *wfBadGuy;

GrVertex p[1000];

SimulationObject *playerSimobj;
SimulationObjectList simObjectList;

SimulationCore::SimulationCore()
{
	simState = STATE_SIMULATING;
	simIsInitialized = false;
}

SimulationCore::~SimulationCore()
{

}

void SimulationCore::FrameCallback()
{
	SimOneFrame();
}

JackCube *enemy;

#ifdef __WINDOWS__
#define BASEPATH C:\\Glide\\gameres
#endif

#ifdef __GNUC__
#define BASEPATH std::string("/Users/luigi/src/gameres")
#endif

#ifdef __VMS
#define BASEPATH std::string("/DKA0/users/luigi/gameres")
#endif

#define RESOURCE_PATH(FILE) ((BASEPATH + std::string(FILE)).c_str())

void SimulationCore::SetupSimulation()
{
	/* Set up the simulation environment. Create our MVP matrix,
	Glide screen, etc. */
	
	if(simIsInitialized) { printf("Tried to initialize simulation again?\n"); return; }

	bmFontMonogram32 = new BMFont(RESOURCE_PATH("/Fonts/monogram.fnt"), RESOURCE_PATH("/Fonts/monogram.bmp"));
	bmFontMonogram16 = new BMFont(RESOURCE_PATH("/Fonts/monogram16.fnt"), RESOURCE_PATH("/Fonts/monogram16.bmp"));

	texMinAddress = grTexMinAddress(GR_TMU0);
	texMaxAddress = grTexMaxAddress(GR_TMU0);

	debugFont = new BMP(RESOURCE_PATH("/Fonts/font8.bmp"), GR_LFB_SRC_FMT_565);

	WavefrontObj *wfEraser = new WavefrontObj(RESOURCE_PATH("/Models/eraser.obj"));
	WavefrontObj *wfCube = new WavefrontObj(RESOURCE_PATH("/Models/cubetextured.obj"));
	//wfBadGuy = new WavefrontObj(RESOURCE_PATH("/Models/enemy.obj"));

	Model3D *mCube = new Model3D("cube", wfCube);
	Model3D *mEraser = new Model3D("eraser", wfEraser);
	//Model3D *mBadGuy = new Model3D("badguy", wfBadGuy);

	textureExample = new GlideTexture(RESOURCE_PATH("/3DF/heeho.3df"));
	textureExample->Download();
	textureExample->SetAsSource();

	font16 = new GlideTexture(RESOURCE_PATH("/3DF/font16.3df"));
	font16->Download();	font8 = new GlideTexture(RESOURCE_PATH("/3DF/font8.3df"));
	font8->Download();
	placeholderLaser = new GlideTexture(RESOURCE_PATH("/3DF/newglo9.3df"));
	placeholderLaser->Download();

	g_Renderer.CreateProjectionMatrix(&g_Renderer.mtxProjection);

	tlSetScreen(800, 600);	// one 800x600 Glide framebuffer
	grClipWindow(0, 0, 800, 600);

	// Set up texture modes...
	grTexFilterMode(GR_TMU0, GR_TEXTUREFILTER_BILINEAR, GR_TEXTUREFILTER_BILINEAR);

	grTexCombine(GR_TMU0,
		GR_COMBINE_FUNCTION_LOCAL,
		GR_COMBINE_FACTOR_NONE,
		GR_COMBINE_FUNCTION_LOCAL,
		GR_COMBINE_FACTOR_NONE,
		FXFALSE, FXFALSE);

	tlConSet(0, 0,
		1, 1,
		80, 40,
		0xFFFFFFFF);

	g_Renderer.CreateProjectionMatrix(&g_Renderer.mtxProjection);

	grCullMode(GR_CULL_NEGATIVE);

	// 1000 random points
	for (int n = 0; n < 1000; n++)
	{
		p[n].x = (float)(rand() % 800);
		p[n].y = (float)(rand() % 600);
		p[n].ooz = 65535.0f/65534.0f;	// 65535 / Z
	}

	grDepthBufferMode(GR_DEPTHBUFFER_ZBUFFER);
	grDepthMask(FXTRUE);
	// 1/Z decreases as Z increases.
	grDepthBufferFunction(GR_CMP_GREATER);

	tlConClear();

	simIsInitialized = true;

	/* test model code */
	SimulationObject *model = new SimulationObject("player", mEraser);
	model->can_be_targeted = true;
	model->is_player = true;
	model->visible = false;
	model->is_physics_object = true;
	model->team = TEAM_GOOD;
	model->physics.max_rotational_velocity = Vector3f(0.5, 0.5, 0.5);
	model->transformation.position.y = 0;
	model->transformation.scale.x = 1.0;
	model->transformation.scale.y = 1.0;
	model->transformation.scale.z = 1.0;
	playerSimobj = model;
	simObjectList.Add(model);
	
	enemy = new JackCube("heeho", mCube);
	enemy->can_be_targeted = true;
	enemy->transformation.position.x = 0;
	enemy->transformation.position.y = 0;
	enemy->transformation.position.z = 20;
	enemy->transformation.rotation.x = 45;
	enemy->transformation.rotation.y = 45;
	enemy->transformation.rotation.z = 45;
	enemy->transformation.scale.x = 2;
	enemy->transformation.scale.y = 2;
	enemy->transformation.scale.z = 2;
	simObjectList.Add(enemy);

	Camera *camera = new Camera("playercamera");
	camera->can_be_targeted = false;
	camera->track_object = model;
	camera->track_offset = Vector3f(0, 0, 0);
	simObjectList.Add(camera);

	g_HudManager.CreateHud();
	
	simIsInitialized = true;
}

#ifdef __ALPHA
#define CPU "DEC Alpha"
#elif defined(__PPC__)
#define CPU "PowerPC"
#elif defined(__X86__)
#define CPU "x86"
#endif

#ifdef __VMS
#define OS "VMS V8.4"
#elif defined(__OSX__)
#define OS "Mac OS X"
#elif defined(__WINDOWS__)
#define OS "MS Windows"
#endif

void SimulationCore::ConsoleDebugInfo(Vector3f eye, Vector3f rot, Vector3f target)
{
	char buildinfo[128];
	sprintf(buildinfo, "Built %s %s (%s %s)\n", __DATE__, __TIME__, CPU, OS);

	// Debug readout.
	tlConClear();
	tlConOutput("Polysim II (Glide 2.56)\n");
	tlConOutput(buildinfo);
	tlConOutput("eye   : %f %f %f\n", eye.x, eye.y, eye.z);
	tlConOutput("rot   : %f %f %f\n", rot.x, rot.y, rot.z);
	tlConOutput("target: %f %f %f\n", target.x, target.y, target.z);
	tlConOutput("mouse : %d %d\n", g_InputManager.mousePosition.x, g_InputManager.mousePosition.y);
	tlConRender();
}

int rotationDegrees = 0;

void doTextureTest()
{
	GrVertex vtxA, vtxB, vtxC, vtxD;
	grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER,
				GR_COMBINE_FACTOR_ONE,
				GR_COMBINE_LOCAL_NONE,
				GR_COMBINE_OTHER_TEXTURE,
				FXFALSE );

	vtxA.oow = 1.0f;
	vtxB = vtxC = vtxD = vtxA;

	vtxA.x = vtxC.x = 0.0f;
	vtxB.x = vtxD.x = 256.0f;
	vtxA.y = vtxB.y = 0.0f;
	vtxC.y = vtxD.y = 256.0f;

	vtxA.tmuvtx[0].sow = vtxC.tmuvtx[0].sow = 0.0f;
	vtxB.tmuvtx[0].sow = vtxD.tmuvtx[0].sow = 255.0f;
	vtxA.tmuvtx[0].tow = vtxB.tmuvtx[0].tow = 255.0f;
	vtxC.tmuvtx[0].tow = vtxD.tmuvtx[0].tow = 0.0f;

	vtxA.r = 255.0f, vtxA.g =   0.0f, vtxA.b =   0.0f, vtxA.a = 255.0f;
	vtxB.r =   0.0f, vtxB.g = 255.0f, vtxB.b =   0.0f, vtxB.a = 128.0f;
	vtxC.r =   0.0f, vtxC.g =   0.0f, vtxC.b = 255.0f, vtxC.a = 128.0f;
	vtxD.r =   0.0f, vtxD.g =   0.0f, vtxD.b =   0.0f, vtxD.a =   0.0f;

    grDrawTriangle( &vtxA, &vtxD, &vtxC );
    grDrawTriangle( &vtxA, &vtxB, &vtxD );
}

void SimulationCore::SimOneFrame()
{
	g_InputManager.UpdateGameInputs();

	/* Process the display list. */
	for(SimulationObjectIterator it = simObjectList.GetBeginning(); 
		it != simObjectList.GetEnding(); 
		++it)
	{
		if((*it)->is_player)
		{
			if((*it)->is_physics_object)
			{
				PhysicsSim::ApplyFlightControls(&(*it)->physics, &(*it)->transformation, &g_InputManager.flightControls);
			}
			(*it)->ApplyInputs();
		}
		(*it)->OnFrame();
	}

	// Update the camera.
	Camera *camera = dynamic_cast<Camera *>(simObjectList.GetSimObject("playercamera"));
	camera->OnFrame();

	Vector3f forward = Vector3f::Forward();
	forward = forward.RotateAroundX(camera->transformation.rotation.x);
	forward = forward.RotateAroundY(camera->transformation.rotation.y);
	forward = forward.RotateAroundZ(camera->transformation.rotation.z);

	g_Renderer.CreateWorldMatrix(&g_Renderer.mtxWorld, camera->transformation.position, camera->transformation.position + forward);

	/* Clear the framebuffer. */
	grBufferClear(0, 0, 0);

	// Starfield points are solid white
	grConstantColorValue(0xFFFFFFFF);
	grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
		GR_COMBINE_LOCAL_CONSTANT, GR_COMBINE_OTHER_NONE, FXFALSE);

	grDepthBufferMode(GR_DEPTHBUFFER_ZBUFFER);
	grDepthMask(FXTRUE);

	for (int i = 0; i < 1000; i++)
	{
		grDrawPoint(&p[i]);
	}

	/* from TEST17.C */
	//doTextureTest();

	//grGlideGetState(&state);

	SimulationObject *jack = simObjectList.GetSimObject("heeho");
	jack->transformation.rotation.x = fmod(jack->transformation.rotation.x, 360.0f) + 1;
	jack->transformation.rotation.y = fmod(jack->transformation.rotation.y, 360.0f) + 1;
	jack->transformation.rotation.z = fmod(jack->transformation.rotation.z, 360.0f) + 1;

	for(SimulationObjectIterator it = simObjectList.GetBeginning(); 
		it != simObjectList.GetEnding(); 
		++it)
	{
		 if((*it)->visible) (*it)->Draw(&g_Renderer.mtxWorld, &g_Renderer.mtxProjection);
	}

/*
	grCullMode(GR_CULL_DISABLE);
	grDepthBufferMode(GR_DEPTHBUFFER_DISABLE);
	grDepthMask(FXTRUE);
	// 1/Z decreases as Z increases.
	grDepthBufferFunction(GR_CMP_ALWAYS);

	GrVertex a, b, c;
	a.x = 100; a.y = 400; a.z = 1;
	b.x = 400; b.y = 400; b.z = 1;
	c.x = 250; c.y = 250; c.z = 1;

	grDrawTriangle(&a, &b, &c);
	grDrawLine(&a, &b);
	grDrawLine(&b, &c);
	grDrawLine(&c, &a);
*/

	g_HudManager.UpdateHud();
	g_HudManager.DrawHud();
	//g_InputManager.DrawMouseCursor();

	this->ConsoleDebugInfo(camera->transformation.position, camera->transformation.rotation, camera->transformation.position + forward);

	bmFontMonogram32->PutString("2D Font", 10, 570);
	bmFontMonogram16->PutString("2D Font", 10, 540);

/*
	Transformation t;
	t.position.x = -5;
	t.position.y = -0;
	t.position.z = 20;
	RenderLaser::Render(&t, 4.0f, FxU32(0xFF0000FF));
*/	
	// 60 fps
	grBufferSwap(1);

	//printf("Frame done\n");
}

/* Simulation setup stuff */

SimulationCore g_SimulationCore;
