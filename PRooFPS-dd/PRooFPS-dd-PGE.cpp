/*
    ###################################################################################
    PRooFPSddPGE.cpp
    Customized PGE for PRooFPS-dd
    Made by PR00F88, West Whiskhyll Entertainment
    2022
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "stdafx.h"  // PCH

#include "PRooFPS-dd-PGE.h"

#include <cassert>

#include "../../../PGE/PGE/PRRE/include/external/PRREuiManager.h"
#include "../../../PGE/PGE/PRRE/include/external/Display/PRREWindow.h"
#include "../../../PGE/PGE/PRRE/include/external/PRRECamera.h"
#include "../../../CConsole/CConsole/src/CConsole.h"


static const std::string GAME_NAME    = "PRooFPS-dd";
static const std::string GAME_VERSION = "0.1.0.0 Alpha";


// ############################### PUBLIC ################################


CPlayer::CPlayer()
{
  health = 100;
  obj = PGENULL;
  pGFX = NULL;
  gravity = 0.0;
  jumping = false;
  canfall = true;
  running = false;
}

void CPlayer::ShutDown()
{
    if ( obj != PGENULL )
    {
        delete obj;
        obj = PGENULL;
    }
    pGFX = NULL;
}

/* be�ll�tja a megjelen�t� objektumot */
void CPlayer::SetRendererObject(PR00FsReducedRenderingEngine* gfx)
{
  pGFX = gfx;
}

/* visszaadja a j�t�kos �leterej�t */
int CPlayer::getHealth() const
{
  return health;
}

PRREVector& CPlayer::getPos1()
{
  return pos;
}

PRREVector& CPlayer::getOPos1()
{
  return oldpos;
}

PRREObject3D* CPlayer::getAttachedObject() const
{
  return obj;
}

void CPlayer::UpdatePositions(const PRREVector& targetPos)
{
    if ( getAttachedObject() != PGENULL )
    {
        getAttachedObject()->getPosVec().Set( getPos1().getX(), getPos1().getY(), getPos1().getZ() );
        getAttachedObject()->getAngleVec().SetY( (targetPos.getX() < 0.f) ? 0.f : 180.f );
    }
}

float CPlayer::getGravity() const
{
  return gravity;
}

bool CPlayer::isJumping() const
{
  return jumping;
}

bool CPlayer::isFalling() const
{
  return ( gravity == 0.0f );
}

bool CPlayer::canFall() const
{
  return canfall;
}

void CPlayer::UpdateOldPos() {
  oldpos = pos;
}

void CPlayer::SetHealth(int value) {
  health = value;
}

void CPlayer::AttachObject(PRREObject3D* value, bool blend) {
  obj = value;   
  if ( obj != PGENULL )
  {  
      obj->SetDoubleSided(true);
      if ( blend )
          obj->getMaterial(false).setBlendFuncs(PRRE_SRC_ALPHA, PRRE_ONE_MINUS_SRC_ALPHA);
      obj->SetLit(false);
  }
}

void CPlayer::SetGravity(float value) {
  gravity = value;
}

void CPlayer::Jump() {
  jumping = true;
  gravity = GAME_GRAVITY_MAX;
  force.SetX( pos.getX()-oldpos.getX() );
  force.SetY( pos.getY()-oldpos.getY() );
  force.SetZ( pos.getZ()-oldpos.getZ() );
}

void CPlayer::StopJumping() {
  jumping = false;
}

void CPlayer::DoDamage(int dmg) {
  health = health - dmg;
  if ( health < 0 ) health = 0;
}

void CPlayer::SetCanFall(bool state) {
  canfall = state;
}

bool CPlayer::isRunning() const
{
    return running;
}

void CPlayer::SetRun(bool state)
{
    running = state;
}

PRREVector& CPlayer::getForce()
{
    return force;
}

void CPlayer::UpdateForce(float x, float y, float z)
{
    force.SetX(x);
    force.SetY(y);
    force.SetZ(z);
}


PRooFPSddPGE* PRooFPSddPGE::createAndGetPRooFPSddPGEinstance()
{
    static PRooFPSddPGE pgeInstance((GAME_NAME + " " + GAME_VERSION).c_str());
    return &pgeInstance;
}


// ############################## PROTECTED ##############################


/**
    This is the only usable ctor, this is used by the static createAndGet().
*/
PRooFPSddPGE::PRooFPSddPGE(const char* gameTitle) :
    PGE(gameTitle),
    m_maps(getPRRE())
{
    m_fps = 0;
    m_fps_counter = 0;
    m_fps_lastmeasure = 0;
    m_fps_ms = 0;

    m_bSpaceReleased = true;
    m_bCtrlReleased = true;
    m_bShiftReleased = true;
    m_enterreleased = true;

    m_bWon = false;
    m_fCameraMinY = 0.0f;
}

PRooFPSddPGE::~PRooFPSddPGE()
{

}

CConsole& PRooFPSddPGE::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
}


const char* PRooFPSddPGE::getLoggerModuleName()
{
    return "PRooFPSddPGE";
}

/**
    Must-have minimal stuff before loading anything.
    Game engine calls this before even finishing its own initialization.
*/
void PRooFPSddPGE::onGameInitializing()
{
    // Earliest we can enable our own logging
    getConsole().Initialize((GAME_NAME + " " + GAME_VERSION + " log").c_str(), true);
    getConsole().SetLoggingState(getLoggerModuleName(), true);
    getConsole().SetFGColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, "999999" );
    getConsole().SetIntsColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, "FFFF00" );
    getConsole().SetStringsColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, "FFFFFF" );
    getConsole().SetFloatsColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, "FFFF00" );
    getConsole().SetBoolsColor( FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, "00FFFF" );

    // PRooFPSddPGE (game) logs
    getConsole().SetLoggingState(getLoggerModuleName(), true);

    // Network logs
    getConsole().SetLoggingState("PGESysNET", true);
    getConsole().SetLoggingState("PgeNetwork", true);
    getConsole().SetLoggingState("PgeServer", true);
    getConsole().SetLoggingState("PgeClient", true);

    // Turn everything on for development only
    getConsole().SetLoggingState("4LLM0DUL3S", true);
}

/** 
    Loading game content here.
*/
void PRooFPSddPGE::onGameInitialized()
{
    getConsole().OLnOI("PRooFPSddPGE::onGameInitialized()");

    getConsole().SetLoggingState("4LLM0DUL3S", false);

    getPRRE().getScreen().SetVSyncEnabled(true);

    getPRRE().getCamera().SetNearPlane(0.1f);
    getPRRE().getCamera().SetFarPlane(100.0f);
    getPRRE().getCamera().getPosVec().Set( 0, 0, GAME_CAM_Z );
    getPRRE().getCamera().getTargetVec().Set( 0, 0, -GAME_BLOCK_SIZE_Z );

    m_maps.initialize();
    assert( m_maps.load("gamedata/maps/map_test_good.txt") );

    m_player.AttachObject( getPRRE().getObject3DManager().createPlane(GAME_PLAYER_W, GAME_PLAYER_H), true );
    m_pTexPlayer = getPRRE().getTextureManager().createFromFile( "gamedata\\textures\\giraffe1m.bmp" );
    m_player.getAttachedObject()->getMaterial().setTexture( m_pTexPlayer );
    m_player.getPos1() = m_maps.getRandomSpawnpoint();

    assert( getWeaponManager().load("gamedata/weapons/machinegun.txt") );

    m_pObjXHair = getPRRE().getObject3DManager().createPlane(32.f, 32.f);
    m_pObjXHair->SetStickedToScreen(true);
    m_pObjXHair->SetDoubleSided(true);
    m_pObjXHair->SetTestingAgainstZBuffer(false);
    m_pObjXHair->SetLit(false);
    // for bitmaps not having proper alpha bits (e.g. saved by irfanview or mspaint), use (PRRE_SRC_ALPHA, PRRE_ONE)
    // otherwise (bitmaps saved by Flash) just use (PRRE_SRC_ALPHA, PRRE_ONE_MINUS_SRC_ALPHA) to utilize real alpha
    m_pObjXHair->getMaterial(false).setBlendFuncs(PRRE_SRC_ALPHA, PRRE_ONE);
    PRRETexture* xhairtex = getPRRE().getTextureManager().createFromFile( "gamedata\\textures\\hud_xhair.bmp" );
    m_pObjXHair->getMaterial().setTexture( xhairtex );

    getPRRE().WriteList();

    if (getNetwork().isServer())
    {
        // MsgUserSetup is also processed by server, but it injects this pkt into its own queue when needed.
        // MsgUserSetup MUST NOT be received by server over network!
        // MsgUserSetup is received only by clients over network!
        getNetwork().getServer().getBlackListedAppMessages().insert(static_cast<pge_network::TPgeMsgAppMsgId>(proofps_dd::MsgUserSetup::id));

        // MsgUserUpdate is also processed by server, but it injects this pkt into its own queue when needed.
        // MsgUserUpdate MUST NOT be received by server over network!
        // MsgUserUpdate is received only by clients over network!
        getNetwork().getServer().getBlackListedAppMessages().insert(static_cast<pge_network::TPgeMsgAppMsgId>(proofps_dd::MsgUserUpdate::id));

        if (!getNetwork().getServer().startListening())
        {
            PGE::showErrorDialog("Server has FAILED to start listening!");
            assert(false);
        }
    }
    else
    {
        getNetwork().getClient().getBlackListedAppMessages().insert(static_cast<pge_network::TPgeMsgAppMsgId>(proofps_dd::MsgUserCmdMove::id));

        if (!getNetwork().getClient().connectToServer("127.0.0.1"))
        {
            PGE::showErrorDialog("Client has FAILED to establish connection to the server!");
            assert(false);
        }
    }

    getConsole().OOOLn("PRooFPSddPGE::onGameInitialized() done!");

    getInput().getMouse().SetCursorPos(
        getPRRE().getWindow().getX() + getPRRE().getWindow().getWidth()/2,
        getPRRE().getWindow().getY() + getPRRE().getWindow().getHeight()/2);
    getPRRE().getWindow().SetCursorVisible(false);
    
    m_fps_lastmeasure = GetTickCount();
    m_fps = 0;
}

//function bulletProc() {
//	this._x += this.xMove * _root.BULLETSPEED;
//	this._y += this.yMove * _root.BULLETSPEED;
//	for (obj in _root.mc_map) {
//		if (typeof(_root.mc_map[obj])=="movieclip"){
//			if ( _root.collides(this._x, this._y, this._width, this._height,
//								_root.mc_map[obj]._x, _root.mc_map[obj]._y, _root.mc_map[obj]._width, _root.mc_map[obj]._height)
//				)
//			{
//				if ( _root.mc_map[obj]._name.substr(0,8) == "mc_snail" ) {
//					if ( _root.mc_map[obj].health > 0 ) {
//						_root.mc_map[obj].health -= _root.BULLETDAMAGE;
//						//trace(_root.mc_map[obj]._name);
//						if ( _root.mc_map[obj].health <= 0 ) {
//							
//							_root.killedSnails++;
//							//trace(_root.killedSnails);
//							_root.gameEnds += 4000;
//							if ( _root.mc_map[obj]._currentframe == 1 ) { _root.mc_map[obj].gotoAndPlay(11); }
//							else if ( _root.mc_map[obj]._currentframe == 2 ) { _root.mc_map[obj].gotoAndPlay(3); }
//						}
//						_root.mc_map.attachMovie("lnk_mc_blood", "mc_blood"+_root.bloodCount, _root.mc_map.getNextHighestDepth());
//						eval("_root.mc_map.mc_blood"+_root.bloodCount)._x = this._x;
//						eval("_root.mc_map.mc_blood"+_root.bloodCount)._y = this._y;
//						_root.bloodCount++;
//						this.onEnterFrame = null;
//						removeMovieClip( this );
//						break;
//					}
//				}
//				if ( _root.mc_map[obj]._name.substr(0,4) == "inst" ) {
//					this.onEnterFrame = null;
//					removeMovieClip( this );
//					break;
//				}
//			}
//		}
//	}
//}

//function Shoot() {
//		_root.mc_bulletpacer.play();
//		var aimShit:Number = _root.randRange(-_root.AIMDIFFICULTY, _root.AIMDIFFICULTY);
//		_root.mc_map.attachMovie("lnk_mc_bullet", "mc_bullet"+_root.bulletCount, _root.mc_map.getNextHighestDepth());
//		eval("_root.mc_map.mc_bullet"+_root.bulletCount)._rotation = this.mc_player_arm1._rotation + aimShit;
//		var neg:Number = 1;
//		if ( this._currentframe == 2 ) { neg = -1 };
//		eval("_root.mc_map.mc_bullet"+_root.bulletCount).xMove = Math.cos((this.mc_player_arm1._rotation+aimShit)*Math.PI/180.0) * neg;
//		eval("_root.mc_map.mc_bullet"+_root.bulletCount).yMove = Math.sin((this.mc_player_arm1._rotation+aimShit)*Math.PI/180.0) * neg;
//		eval("_root.mc_map.mc_bullet"+_root.bulletCount)._x = this.newX + eval("_root.mc_map.mc_bullet"+_root.bulletCount).xMove*this.mc_player_arm1._width;
//		eval("_root.mc_map.mc_bullet"+_root.bulletCount)._y = this.newY+150 + eval("_root.mc_map.mc_bullet"+_root.bulletCount).yMove*this.mc_player_arm1._width;
//		eval("_root.mc_map.mc_bullet"+_root.bulletCount).onEnterFrame = _root.bulletProc;
//		_root.bulletCount++;
//		this.ammo--;
//	}

/** 
    Freeing up game content here.
    Free up everything that has been allocated in onGameInitialized() and onGameRunning().
*/
void PRooFPSddPGE::onGameDestroying()
{
    getConsole().OLnOI("PRooFPSddPGE::onGameDestroying() ...");
    m_maps.shutdown();
    getPRRE().getObject3DManager().DeleteAll();
    getPRRE().getWindow().SetCursorVisible(true);

    getConsole().OOOLn("PRooFPSddPGE::onGameDestroying() done!");
    getConsole().Deinitialize();
}

void PRooFPSddPGE::KeyBoard(int /*fps*/, bool& won)
{
    const PGEInputKeyboard& keybd = getInput().getKeyboard();
  float speed;

  if ( m_player.isRunning() )
      speed = GAME_PLAYER_SPEED2/60.0f;
  else
      speed = GAME_PLAYER_SPEED1/60.0f;
  
  if ( keybd.isKeyPressed(VK_ESCAPE) )
  {
      getPRRE().getWindow().Close();
  }
      
  if ( !won )
  {
    if ( keybd.isKeyPressed( VK_LEFT ) || keybd.isKeyPressed((unsigned char)VkKeyScan('a')) )
    {
        if ( !m_player.isJumping() && !m_player.isFalling() && m_bAllowJump )
        {
            m_player.getPos1().SetX( m_player.getPos1().getX() - speed );
        }
    }
    if ( keybd.isKeyPressed( VK_RIGHT ) || keybd.isKeyPressed((unsigned char)VkKeyScan('d')) )
    {
        if ( !m_player.isJumping() && !m_player.isFalling() && m_bAllowJump )
        {
            m_player.getPos1().SetX( m_player.getPos1().getX() + speed );
        }
    }

    if ( keybd.isKeyPressed( VK_SPACE ) )
    {
        if ( m_bSpaceReleased )
        {
            if ( !m_player.isJumping() && m_bAllowJump && !m_player.isFalling() )
            {
                m_player.Jump();
                
                m_bSpaceReleased = false;
            }
        }
    }
    else
        m_bSpaceReleased = true;

    if ( keybd.isKeyPressed( VK_CONTROL ) )
    {
          if ( m_bCtrlReleased )
          {
            //BulletManager->Create(Player->GetX(),Player->GetY()-0.15,Player->GetZ(),-1);
            m_bCtrlReleased = false;
          }
    }
    else 
       m_bCtrlReleased = true;

    if ( keybd.isKeyPressed( VK_SHIFT ) )
    {
          if ( m_bShiftReleased )
          {
            //BulletManager->Create(Player->GetX(),Player->GetY()-0.15,Player->GetZ(),-1);
            m_bShiftReleased = false;
            if ( m_player.isRunning() )
                m_player.SetRun(false);
            else
                m_player.SetRun(true);
          }
    }
    else 
       m_bShiftReleased = true;
  }
  else
  {
        if ( keybd.isKeyPressed( VK_RETURN ) )
        {
              if ( m_enterreleased )
              {

              }
        }
        else 
           m_enterreleased = true;  
  } // won
}


void PRooFPSddPGE::Mouse(int /*fps*/, bool& /*won*/)
{
    PGEInputMouse& mouse = getInput().getMouse();

    const int oldmx = mouse.getCursorPosX();
    const int oldmy = mouse.getCursorPosY();

    mouse.SetCursorPos(
        getPRRE().getWindow().getX() + getPRRE().getWindow().getWidth()/2,
        getPRRE().getWindow().getY() + getPRRE().getWindow().getHeight()/2);

    const int dx = oldmx - mouse.getCursorPosX();
    const int dy = oldmy - mouse.getCursorPosY();

    m_pObjXHair->getPosVec().Set(
        m_pObjXHair->getPosVec().getX() + dx,
        m_pObjXHair->getPosVec().getY() - dy,
        0.f);

    if ( mouse.isButtonPressed(PGEInputMouse::MouseButton::MBTN_LEFT) )
    {
        // TODO: obviously we will need a getActiveWeapon() for WeaponManager and its shoot() should be called
        getWeaponManager().getWeapons()[0].shoot();
    }
}


// k�t adott objektum �tk�zik-e egym�ssal
bool PRooFPSddPGE::Colliding(PRREObject3D& a, PRREObject3D& b)
{
  if (
       ( 
         (a.getPosVec().getX() - a.getSizeVec().getX()/2 <= b.getPosVec().getX() + b.getSizeVec().getX()/2)
           &&
         (a.getPosVec().getX() + a.getSizeVec().getX()/2 >= b.getPosVec().getX() - b.getSizeVec().getX()/2)
       )
         &&
       ( 
         (a.getPosVec().getY() - a.getSizeVec().getY()/2 <= b.getPosVec().getY() + b.getSizeVec().getY()/2)
           &&
         (a.getPosVec().getY() + a.getSizeVec().getY()/2 >= b.getPosVec().getY() - b.getSizeVec().getY()/2)
       )
         &&
       ( 
         (a.getPosVec().getZ() - a.getSizeVec().getZ()/2 <= b.getPosVec().getZ() + b.getSizeVec().getZ()/2)
           &&
         (a.getPosVec().getZ() + a.getSizeVec().getZ()/2 >= b.getPosVec().getZ() - b.getSizeVec().getZ()/2)
       )
     )
     {
       return true;
     }
    else
     {
       return false; 
     }
}       

// k�t t�rbeli ter�let �tk�zik-e egym�ssal
bool PRooFPSddPGE::Colliding2( float o1px, float o1py, float o1pz, float o1sx, float o1sy, float o1sz,
                      float o2px, float o2py, float o2pz, float o2sx, float o2sy, float o2sz )
{
  if (
       ( 
         (o1px - o1sx/2 <= o2px + o2sx/2)
           &&
         (o1px + o1sx/2 >= o2px - o2sx/2)
       )
         &&
       ( 
         (o1py - o1sy/2 <= o2py + o2sy/2)
           &&
         (o1py + o1sy/2 >= o2py - o2sy/2)
       )
         &&
       ( 
         (o1pz - o1sz/2 <= o2pz + o2sz/2)
           &&
         (o1pz + o1sz/2 >= o2pz - o2sz/2)
       )
     )
     {
       return true;
     }
    else
     {
       return false; 
     }              
}

// a m_player �tk�z�seit kezeli
void PRooFPSddPGE::Collision(bool& /*won*/)
{ 
    PRREObject3D* plobj = m_player.getAttachedObject();
    PRREVector pos, oldpos;
    pos = m_player.getOPos1();
    oldpos = m_player.getPos1();

    m_player.getPos1().SetX( m_player.getPos1().getX() + m_player.getForce().getX() );

    if ( m_player.getOPos1().getY() != m_player.getPos1().getY() )
    {
        for (int i = 0; i < getPRRE().getObject3DManager().getSize(); i++)
        {
            PRREObject3D* obj = (PRREObject3D*) getPRRE().getObject3DManager().getAttachedAt(i);
            if ( (obj != PGENULL) && (obj != plobj) && (obj->isColliding_TO_BE_REMOVED()) )
            {
              if ( Colliding2( obj->getPosVec().getX(), obj->getPosVec().getY(), obj->getPosVec().getZ(), obj->getSizeVec().getX(), obj->getSizeVec().getY(), obj->getSizeVec().getZ(),
                               m_player.getOPos1().getX(), m_player.getPos1().getY(), m_player.getOPos1().getZ(), plobj->getSizeVec().getX(), plobj->getSizeVec().getY(), plobj->getSizeVec().getZ() )
                 )
              {  
                m_player.getPos1().SetY( m_player.getOPos1().getY() );
                if ( obj->getPosVec().getY()+obj->getSizeVec().getY()/2 <= m_player.getPos1().getY()-GAME_PLAYER_H/2.0f + 0.01f )
                {
                  m_player.SetCanFall( false );
                  m_player.getPos1().SetY( obj->getPosVec().getY()+obj->getSizeVec().getY()/2 + GAME_PLAYER_H/2.0f + 0.01f);
                  m_player.UpdateForce(0.0f, 0.0f, 0.0f);
                }
                else
                {
                    m_player.SetCanFall( true );
                    
                }
                break;
              }
            }
        }
    }

    if ( m_player.getOPos1().getX() != m_player.getPos1().getX() )
    {
        for (int i = 0; i < getPRRE().getObject3DManager().getSize(); i++)
          {
            PRREObject3D* obj = (PRREObject3D*) getPRRE().getObject3DManager().getAttachedAt(i);
            if ( (obj != PGENULL) && (obj != plobj) && (obj->isColliding_TO_BE_REMOVED()) )
            {
              if ( Colliding2( obj->getPosVec().getX(), obj->getPosVec().getY(), obj->getPosVec().getZ(), obj->getSizeVec().getX(), obj->getSizeVec().getY(), obj->getSizeVec().getZ(),
                               m_player.getPos1().getX(), m_player.getPos1().getY(), m_player.getOPos1().getZ(), plobj->getSizeVec().getX(), plobj->getSizeVec().getY(), plobj->getSizeVec().getZ() )
                 )
              {
                m_player.getPos1().SetX( m_player.getOPos1().getX() );
                break;
              }
            }
          }
    }
}

void PRooFPSddPGE::CameraMovement(int /*fps*/)
{
    PRREVector campos = getPRRE().getCamera().getPosVec();
    float celx, cely;
    float speed = GAME_CAM_SPEED / 60.0f;

    /* ne mehessen t�ls�gosan balra vagy jobbra a kamera */
    //if ( m_player.getPos1().getX() < m_maps.getStartPos().getX() )
    //    celx = m_maps.getStartPos().getX();
    //else
    //    if ( m_player.getPos1().getX() > m_maps.getEndPos().getX() )
    //        celx = m_maps.getEndPos().getX();
    //     else
            celx = m_player.getPos1().getX();

    /* ne mehessen t�ls�gosan le �s fel a kamera */
    //if ( m_player.getPos1().getY() < m_fCameraMinY )
    //    cely = m_fCameraMinY;
    //else
    //    if ( m_player.getPos1().getY() > GAME_CAM_MAX_Y )
    //        cely = GAME_CAM_MAX_Y;
    //    else
            cely = m_player.getPos1().getY();

    /* a j�t�koshoz igaz�tjuk a kamer�t */
    if ( m_player.getPos1().getX() != campos.getX() )
    {
        campos.SetX(campos.getX() + ((celx - campos.getX())/speed) );
    }
    if ( m_player.getPos1().getY() != campos.getY() )
    {
        campos.SetY(campos.getY() + ((cely - campos.getY())/speed) );
    }

    getPRRE().getCamera().getPosVec().Set( campos.getX(), campos.getY(), GAME_CAM_Z );
    getPRRE().getCamera().getTargetVec().Set( campos.getX(), campos.getY(), m_player.getPos1().getZ() );

} // CameraMovement()


void PRooFPSddPGE::Gravity(int fps)
{
  if ( m_player.isJumping() )
  {
    m_player.SetGravity( m_player.getGravity()-GAME_JUMPING_SPEED/(float)fps );
    if ( m_player.getGravity() < 0.0 )
    {
      m_player.SetGravity( 0.0 );
      m_player.StopJumping();
    }
  }
  else
  {
    if ( m_player.getGravity() > GAME_GRAVITY_MIN )
    {
      m_player.SetGravity( m_player.getGravity()-GAME_FALLING_SPEED/(float)fps );
      if ( m_player.getGravity() < GAME_GRAVITY_MIN ) m_player.SetGravity( GAME_GRAVITY_MIN );
    }
  }
  m_player.getPos1().SetY( m_player.getPos1().getY() + m_player.getGravity() );
  if ( m_player.getPos1().getY() < m_maps.getObjectsMinY()-5.0f )
      m_player.SetHealth(0);
}

void PRooFPSddPGE::FrameLimiter(int fps_ms)
{
  //if ( GAME_MAXFPS > 0 )
  {
    if ( (1000/(float)GAME_MAXFPS) - fps_ms > 0.0f )
        Sleep(PFL::roundi((1000/(float)GAME_MAXFPS) - fps_ms));
    else {
        Sleep(1);
    }
  }
}


void PRooFPSddPGE::UpdateBullets()
{
    // on the long run this function needs to be part of the game engine itself, however currently game engine doesn't handle collisions,
    // so once we introduce the collisions to the game engine, it will be an easy move of this function as well there
    std::list<Bullet>& bullets = getWeaponManager().getBullets();
    for (auto& bullet : bullets)
    {
        bullet.Update();
    }
}

/** 
    Game logic here.
    Game engine invokes this in every frame.
    DO NOT make any unnecessary operations here, as this function must always complete below 16 msecs to keep stable 60 fps!
*/
void PRooFPSddPGE::onGameRunning()
{
    PRREWindow& window = getPRRE().getWindow();

    if ( m_fps == 0 ) {
        m_fps = 60;
    }
    m_fps_ms = GetTickCount();

    if ( m_player.getPos1().getY() != m_player.getOPos1().getY() )
    { // el�z� frame-ben m�g tudott zuhanni, teh�t egyel�re nem ugorhatunk
        m_bAllowJump = false;
    }
    else
    {
        m_bAllowJump = true;
    }

    m_player.UpdateOldPos();
    KeyBoard(m_fps, m_bWon);
    Mouse(m_fps, m_bWon);
    if ( !m_bWon)
    {
        Gravity(m_fps);
        Collision(m_bWon);
    }
    CameraMovement(m_fps);
    m_player.UpdatePositions( m_pObjXHair->getPosVec() );
    // TODO: obviously we will need a getActiveWeapon() for WeaponManager
    Weapon& wpn = getWeaponManager().getWeapons()[0];
    wpn.UpdatePositions(m_player.getPos1(), m_pObjXHair->getPosVec());   
    wpn.Update();
    UpdateBullets();

    //map.UpdateVisibilitiesForRenderer();

    // k�pkockasz�m limit�l� (akkor kell, ha nincs vsync)
    m_fps_ms = GetTickCount() - m_fps_ms;
    //FrameLimiter(m_fps_ms);
    // fps m�r� friss�t�se 
    m_fps_counter++;
    if ( GetTickCount() - GAME_FPS_INTERVAL >= m_fps_lastmeasure )
    {
        m_fps = m_fps_counter * (1000/GAME_FPS_INTERVAL);
        m_fps_counter = 0;
        m_fps_lastmeasure = GetTickCount();
    } 

    std::stringstream str;
    str << GAME_NAME << " " << GAME_VERSION << " :: " << wpn.getMagBulletCount() << " / " << wpn.getUnmagBulletCount() << " :: FPS: " << m_fps << " :: angleZ: " << wpn.getObject3D().getAngleVec().getZ();
    window.SetCaption(str.str());
}


// ############################### PRIVATE ###############################


void PRooFPSddPGE::genUniqueUserName(char szNewUserName[proofps_dd::MsgUserSetup::nUserNameMaxLength]) const
{
    bool found = false;
    do
    {
        sprintf_s(szNewUserName, proofps_dd::MsgUserSetup::nUserNameMaxLength, "User%d", 10000 + (rand() % 100000));
        for (const auto& client : m_mapPlayers)
        {
            found = (client.first == szNewUserName);
            if (found)
            {
                break;
            }
        }
    } while (found);
}

void PRooFPSddPGE::WritePlayerList()
{
    getConsole().OLnOI("PRooFPSddPGE::%s()", __func__);
    for (const auto& player : m_mapPlayers)
    {
        getConsole().OLn("Username: %s; connHandleServerSide: %u; address: %s",
            player.first.c_str(), player.second.m_connHandleServerSide, player.second.m_sIpAddress.c_str());
    }
    getConsole().OO();
}

void PRooFPSddPGE::HandleUserSetup(pge_network::PgeNetworkConnectionHandle connHandleServerSide, const proofps_dd::MsgUserSetup& msg)
{
    if ((strnlen(msg.m_szUserName, proofps_dd::MsgUserSetup::nUserNameMaxLength) > 0) && (m_mapPlayers.end() != m_mapPlayers.find(msg.m_szUserName)))
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): cannot happen: user %s (connHandleServerSide: %u) is already present in players list!",
            __func__, msg.m_szUserName, connHandleServerSide);
        assert(false);
        return;
    }

    if (msg.m_bCurrentClient)
    {
        getConsole().OLn("PRooFPSddPGE::%s(): this is me, my name is %s, connHandleServerSide: %u, my IP: %s",
            __func__, msg.m_szUserName, connHandleServerSide, msg.m_szIpAddress);
        // store our username so we can refer to it anytime later
        m_sUserName = msg.m_szUserName;

        if (getNetwork().isServer())
        {
            getPRRE().getUImanager().addText("Server, User name: " + m_sUserName, 10, 30);
        }
        else
        {
            getPRRE().getUImanager().addText("Client, User name: " + m_sUserName + "; IP: " + msg.m_szIpAddress, 10, 30);
        }
    }
    else
    {
        getConsole().OLn("PRooFPSddPGE::%s(): new user %s (connHandleServerSide: %u; IP: %s) connected",
            __func__, msg.m_szUserName, connHandleServerSide, msg.m_szIpAddress);
    }

    // insert user into map using wacky syntax
    m_mapPlayers[msg.m_szUserName];
    m_mapPlayers[msg.m_szUserName].m_connHandleServerSide = connHandleServerSide;
    m_mapPlayers[msg.m_szUserName].m_sIpAddress = msg.m_szIpAddress;

    PRREObject3D* const plane = getPRRE().getObject3DManager().createPlane(0.5f, 0.5f);
    if (!plane)
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): failed to create object for user %s!", __func__, msg.m_szUserName);
        // TODO: should exit or sg
        return;
    }

    plane->SetDoubleSided(true);
    plane->getPosVec().SetX(0);
    plane->getPosVec().SetZ(2);

    plane->setVertexModifyingHabit(PRRE_VMOD_STATIC);
    plane->setVertexReferencingMode(PRRE_VREF_INDEXED);

    m_mapPlayers[msg.m_szUserName].m_pObject3D = plane;

    getNetwork().WriteList();
    WritePlayerList();
}

void PRooFPSddPGE::HandleUserConnected(pge_network::PgeNetworkConnectionHandle connHandleServerSide, const pge_network::MsgUserConnected& msg)
{
    if (!getNetwork().isServer())
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): client received MsgUserConnected, CANNOT HAPPEN!", __func__);
        assert(false);
        return;
    }

    const char* szConnectedUserName = nullptr;

    if (msg.bCurrentClient)
    {
        // server is processing its own birth
        if (m_mapPlayers.size() == 0)
        {
            char szNewUserName[proofps_dd::MsgUserSetup::nUserNameMaxLength];
            genUniqueUserName(szNewUserName);
            getConsole().OLn("PRooFPSddPGE::%s(): first (local) user %s connected and I'm server, so this is me (connHandleServerSide: %u)",
                __func__, szNewUserName, connHandleServerSide);

            szConnectedUserName = szNewUserName;

            pge_network::PgePacket newPktSetup;
            proofps_dd::MsgUserSetup::initPkt(newPktSetup, connHandleServerSide, true, szConnectedUserName, msg.szIpAddress);

            // server injects this msg to self so resources for player will be allocated
            getNetwork().getServer().getPacketQueue().push_back(newPktSetup);
        }
        else
        {
            // cannot happen
            getConsole().EOLn("PRooFPSddPGE::%s(): user (connHandleServerSide: %u) connected with bCurrentClient as true but it is not me, CANNOT HAPPEN!",
                __func__, connHandleServerSide);
            assert(false);
            return;
        }
    }
    else
    {
        // server is processing another user's birth
        if (m_mapPlayers.empty())
        {
            // cannot happen because at least the user of the server should be in the map!
            // this should happen only if we are dedicated server but currently only listen-server is supported!
            getConsole().EOLn("PRooFPSddPGE::%s(): non-server user (connHandleServerSide: %u) connected but map of players is still empty, CANNOT HAPPEN!",
                __func__, connHandleServerSide);
            assert(false);
            return;
        }

        char szNewUserName[proofps_dd::MsgUserSetup::nUserNameMaxLength];
        genUniqueUserName(szNewUserName);
        szConnectedUserName = szNewUserName;
        getConsole().OLn("PRooFPSddPGE::%s(): new remote user %s (connHandleServerSide: %u) connected (from %s) and I'm server",
            __func__, szConnectedUserName, connHandleServerSide, msg.szIpAddress);

        pge_network::PgePacket newPktSetup;
        proofps_dd::MsgUserSetup::initPkt(newPktSetup, connHandleServerSide, false, szConnectedUserName, msg.szIpAddress);

        // server injects this msg to self so resources for player will be allocated
        getNetwork().getServer().getPacketQueue().push_back(newPktSetup);

        // inform all other clients about this new user
        getNetwork().getServer().SendPacketToAllClients(newPktSetup, connHandleServerSide);

        // now we send this msg to the client with this bool flag set so client will know it is their connect
        proofps_dd::MsgUserSetup& msgUserSetup = reinterpret_cast<proofps_dd::MsgUserSetup&>(newPktSetup.msg.app.cData);
        msgUserSetup.m_bCurrentClient = true;
        getNetwork().getServer().SendPacketToClient(connHandleServerSide, newPktSetup);

        // we also send as many MsgUserSetup pkts to the client as the number of already connected players,
        // otherwise client won't know about them, so this way the client will detect them as newly connected users;
        // we also send MsgUserUpdate about each player so new client will immediately have their positions updated.
        pge_network::PgePacket newPktUserUpdate;
        for (const auto& it : m_mapPlayers)
        {
            proofps_dd::MsgUserSetup::initPkt(
                newPktSetup,
                it.second.m_connHandleServerSide,
                false,
                it.first, it.second.m_sIpAddress);
            getNetwork().getServer().SendPacketToClient(connHandleServerSide, newPktSetup);

            proofps_dd::MsgUserUpdate::initPkt(
                newPktUserUpdate,
                it.second.m_connHandleServerSide,
                it.second.m_pObject3D->getPosVec().getX(), it.second.m_pObject3D->getPosVec().getY(), it.second.m_pObject3D->getPosVec().getZ());
            getNetwork().getServer().SendPacketToClient(connHandleServerSide, newPktUserUpdate);
        }
    }
}

void PRooFPSddPGE::HandleUserDisconnected(pge_network::PgeNetworkConnectionHandle connHandleServerSide, const pge_network::MsgUserDisconnected&)
{
    auto it = m_mapPlayers.begin();
    while (it != m_mapPlayers.end())
    {
        if (it->second.m_connHandleServerSide == connHandleServerSide)
        {
            break;
        }
        it++;
    }

    if (m_mapPlayers.end() == it)
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): failed to find user with connHandleServerSide: %u!", __func__, connHandleServerSide);
        return;
    }

    const std::string& sClientUserName = it->first;

    if (getNetwork().isServer())
    {
        getConsole().OLn("PRooFPSddPGE::%s(): user %s disconnected and I'm server", __func__, sClientUserName.c_str());
    }
    else
    {
        getConsole().OLn("PRooFPSddPGE::%s(): user %s disconnected and I'm client", __func__, sClientUserName.c_str());
    }

    if (it->second.m_pObject3D)
    {
        delete it->second.m_pObject3D;  // yes, dtor will remove this from its Object3DManager too!
    }

    m_mapPlayers.erase(it);

    getNetwork().WriteList();
    WritePlayerList();
}

void PRooFPSddPGE::HandleUserCmdMove(pge_network::PgeNetworkConnectionHandle connHandleServerSide, const proofps_dd::MsgUserCmdMove& pktUserCmdMove)
{
    if (!getNetwork().isServer())
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): client received MsgUserCmdMove, CANNOT HAPPEN!", __func__);
        assert(false);
        return;
    }

    auto it = m_mapPlayers.begin();
    while (it != m_mapPlayers.end())
    {
        if (it->second.m_connHandleServerSide == connHandleServerSide)
        {
            break;
        }
        it++;
    }

    if (m_mapPlayers.end() == it)
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): failed to find user with connHandleServerSide: %u!", __func__, connHandleServerSide);
        return;
    }

    const std::string& sClientUserName = it->first;

    PRREObject3D* obj = it->second.m_pObject3D;
    if (!obj)
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): user %s doesn't have associated Object3D!", __func__, sClientUserName.c_str());
        return;
    }

    if ((pktUserCmdMove.m_dirHorizontal == proofps_dd::HorizontalDirection::NONE) &&
        (pktUserCmdMove.m_dirVertical == proofps_dd::VerticalDirection::NONE))
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): user %s sent invalid cmdMove!", __func__, sClientUserName.c_str());
        return;
    }

    //getConsole().OLn("PRooFPSddPGE::%s(): user %s sent valid cmdMove", __func__, sClientUserName.c_str());
    switch (pktUserCmdMove.m_dirHorizontal)
    {
    case proofps_dd::HorizontalDirection::LEFT:
        obj->getPosVec().SetX(obj->getPosVec().getX() - 0.01f);
        break;
    case proofps_dd::HorizontalDirection::RIGHT:
        obj->getPosVec().SetX(obj->getPosVec().getX() + 0.01f);
        break;
    default: /* no-op */
        break;
    }

    switch (pktUserCmdMove.m_dirVertical)
    {
    case proofps_dd::VerticalDirection::DOWN:
        obj->getPosVec().SetY(obj->getPosVec().getY() - 0.01f);
        break;
    case proofps_dd::VerticalDirection::UP:
        obj->getPosVec().SetY(obj->getPosVec().getY() + 0.01f);
        break;
    default: /* no-op */
        break;
    }

    pge_network::PgePacket pktOut;
    proofps_dd::MsgUserUpdate::initPkt(pktOut, connHandleServerSide, obj->getPosVec().getX(), obj->getPosVec().getY(), obj->getPosVec().getZ());
    getNetwork().getServer().SendPacketToAllClients(pktOut);
    // this msgUserUpdate should be also sent to server as self
    // maybe the SendPacketToAllClients() should be enhanced to contain packet injection for server's packet queue!
    getNetwork().getServer().getPacketQueue().push_back(pktOut);
}

void PRooFPSddPGE::HandleUserUpdate(pge_network::PgeNetworkConnectionHandle connHandleServerSide, const proofps_dd::MsgUserUpdate& msg)
{
    auto it = m_mapPlayers.begin();
    while (it != m_mapPlayers.end())
    {
        if (it->second.m_connHandleServerSide == connHandleServerSide)
        {
            break;
        }
        it++;
    }

    if (m_mapPlayers.end() == it)
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): failed to find user with connHandleServerSide: %u!", __func__, connHandleServerSide);
        return;
    }

    PRREObject3D* obj = it->second.m_pObject3D;
    if (!obj)
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): user %s doesn't have associated Object3D!", __func__, it->first.c_str());
        return;
    }

    obj->getPosVec().SetX(msg.m_pos.x);
    obj->getPosVec().SetY(msg.m_pos.y);
}