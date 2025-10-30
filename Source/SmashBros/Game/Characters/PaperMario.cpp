
#include "PaperMario.h"
#include "../../Global.h"
#include "../../AttackTemplates.h"
#include "../../P2PDataManager.h"
#include "../../Item.h"

namespace SmashBros
{
	void PaperMario::addProjectileInfo(byte type, int projID, float x, float y)
	{
		ProjectileInfo info;
		info.type = type;
		info.projID = projID;
		info.x = x;
		info.y = y;
		createdProjectiles.add(info);
	}
	
	PaperMario::PaperMario(float x1, float y1, byte playerNo, byte team) : Player(x1, y1, playerNo, team)
	{
		charging_attack = 0;
		chargedDownAttack = false;
		chargeDownTime = 0;
		
		walkSpeed = 2.8;  // Slightly slower than Mario
		runSpeed = 3.7;
		fallWalk = 1.8;
		fallRun = 1.8;
		
		weight = 0.08;  // Lighter (paper!)
		
		name = "Paper Mario";
		
		setScale(0.5f);  // Reduced scale to match Mario's size (PaperMario sprites are larger)
		
		setItemOffset(4, 2);
		
		setHitbox(-10, -9, 20, 28);
		setHitboxColor(Color::GREEN);
		//showHitboxWireframe(true);
		//setWireframeColor(Color.red);
		//showWireframe(true);
		
		setHangPoint(5, 20);
		
		Console::WriteLine((String)"finished creating player " + playerNo);
	}

	PaperMario::~PaperMario()
	{
		//
	}
	
	void PaperMario::setToDefaultValues()
	{
		charging_attack = 0;
	}
	
	void PaperMario::Load()
	{
		String folderPath = "Images/Game/Characters/PaperMario/";
		setFolderPath(folderPath);
		
		loadFile(folderPath + "fireball.png");
		loadFile(folderPath + "fludd_water.png");
		loadFile(folderPath + "finalsmash_fire.png");
		addTwoSidedAnimation("stand", "stand.png", 10, 9, 1);
		addTwoSidedAnimation("walk", "walk.png", 10, 4, 1);
		addTwoSidedAnimation("run", "run.png", 15, 8, 1);
		addTwoSidedAnimation("jump", "jump.png", 10, 6, 1);
		addTwoSidedAnimation("jump2", "jump2.png", 30, 20, 1);
		addTwoSidedAnimation("land", "land.png", 20, 3, 1);
		addTwoSidedAnimation("skid", "skid.png", 30, 1, 1);
		addTwoSidedAnimation("fall", "fall.png", 30, 2, 1);
		addTwoSidedAnimation("hang", "hang.png", 1, 1, 1);
		addTwoSidedAnimation("crouch", "crouch.png", 1, 1, 1);
		addTwoSidedAnimation("grab", "grab.png", 10, 4, 1);
		addTwoSidedAnimation("grabbed", "grabbed.png", 30, 1, 1);
		addTwoSidedAnimation("hold", "hold.png", 30, 1, 1);
		addTwoSidedAnimation("release", "release.png", 12, 4, 1);
		addTwoSidedAnimation("toss", "toss.png", 14, 4, 1);
		addTwoSidedAnimation("toss_up", "toss_up.png", 17, 5, 1);
		addTwoSidedAnimation("toss_down", "toss_down.png", 20, 6, 1);
		addTwoSidedAnimation("grab_attack", "grab_attack.png", 10, 3, 1);
		addTwoSidedAnimation("grab_attack_swing", "grab_attack_swing.png", 18, 13, 1);
		addTwoSidedAnimation("melee_weapon", "melee_weapon.png", 8, 4, 1);
		addTwoSidedAnimation("melee_weapon_up", "melee_weapon_up.png", 8, 4, 1);
		addTwoSidedAnimation("melee_weapon_down", "melee_weapon_down.png", 8, 4, 1);
		addTwoSidedAnimation("hurt_minor", "hurt_minor.png", 2, 1, 1);
		addTwoSidedAnimation("hurt_fly", "hurt_fly.png", 2, 1, 1);
		addTwoSidedAnimation("hurt_flip", "hurt_flip.png", 10, 4, 1);
		addTwoSidedAnimation("hurt_spin", "hurt_spin.png", 18, 9, 1);
		addSingleAnimation("hurt_spin_up", "hurt_spin_up.png", 18, 9, 1);
		addTwoSidedAnimation("dash_attack", "dash_attack.png", 17, 6, 1);
		addTwoSidedAnimation("standard_attack", "standard_attack.png", 24, 6, 1);
		addTwoSidedAnimation("standard_attack2", "standard_attack2.png", 26, 7, 1);
		addTwoSidedAnimation("standard_attack3", "standard_attack3.png", 18, 6, 1);
		addTwoSidedAnimation("standard_attack_side", "standard_attack_side.png", 16, 9, 1);
		addTwoSidedAnimation("standard_attack_up", "standard_attack_up.png", 18, 6, 1);
		addTwoSidedAnimation("standard_attack_down", "standard_attack_down.png", 25, 12, 1);
		addTwoSidedAnimation("air_attack", "air_attack.png", 8, 3, 1);
		addTwoSidedAnimation("air_attack_side", "air_attack_side.png", 12, 6, 1);
		addTwoSidedAnimation("air_attack_up", "air_attack_up.png", 45, 20, 1);
		addSingleAnimation("air_attack_down", "air_attack_down.png", 20, 14, 1);
		addTwoSidedAnimation("special_attack", "special_attack.png", 14, 4, 1);
		addTwoSidedAnimation("special_attack_side", "special_attack_side.png", 18, 8, 1);
		addTwoSidedAnimation("special_attack_up", "special_attack_up.png", 7, 4, 1);
		addTwoSidedAnimation("special_charge_down", "special_charge_down.png", 0, 1, 1);
		addTwoSidedAnimation("special_attack_down", "special_attack_down.png", 0, 1, 1);
		addTwoSidedAnimation("smash_charge", "smash_charge.png", 1, 1, 1);
		addTwoSidedAnimation("smash_attack", "smash_attack.png", 10, 4, 1);
		addTwoSidedAnimation("smash_attack_up", "smash_attack_up.png", 8, 4, 1);
		addTwoSidedAnimation("smash_attack_down", "smash_attack_down.png", 10, 4, 1);
		
		// Samurai Final Smash animations
		// Begin: Transform into samurai
		addTwoSidedAnimation("finalsmash_begin", "finalsmash_samurai_begin.png", 10, 13, 1);
		
		// Hold: Fan-wielding action poses (multiple frames for dynamic attack)
		Animation*anim;
		
		// Create samurai attack sequence with fan poses (5 frames from the sprite sheet)
		int samuraiAttackSeq[] = {0, 1, 2, 3, 4}; // Fan-wielding frames (5 total)
		int samuraiSeqTotal = sizeof(samuraiAttackSeq)/sizeof(int);
		
		anim = new Animation("finalsmash_samurai_left", 20);
		for(int i=0; i<samuraiSeqTotal; i++)
		{
			String framename = folderPath + "finalsmash_samurai_hold" + samuraiAttackSeq[i] + ".png";
			anim->addFrame(framename);
		}
		addAnimation(anim);
		
		anim = new Animation("finalsmash_samurai_right", 20);
		for(int i=0; i<samuraiSeqTotal; i++)
		{
			String framename = folderPath + "finalsmash_samurai_hold" + samuraiAttackSeq[i] + ".png";
			anim->addFrame(framename);
		}
		anim->mirror(true);
		addAnimation(anim);
		
		// End: Return to normal
		addTwoSidedAnimation("finalsmash_end", "finalsmash_samurai_end.png", 15, 18, 1);
	}
	
	void PaperMario::LoadAttackTypes()
	{
		addAIAttackType(ATTACK_A,         ATTACKTYPE_MELEE, 6);
		addAIAttackType(ATTACK_SIDEA,     ATTACKTYPE_MELEE, 3);
		addAIAttackType(ATTACK_UPA,       ATTACKTYPE_UPMELEE, 1);
		addAIAttackType(ATTACK_DOWNA,     ATTACKTYPE_DOWNMELEE, 1);
		addAIAttackType(ATTACK_DOWNA,     ATTACKTYPE_MELEE, 1);
		addAIAttackType(ATTACK_B,         ATTACKTYPE_PROJECTILE, 1);
		addAIAttackType(ATTACK_SIDEB,     ATTACKTYPE_MELEE, 1);
		addAIAttackType(ATTACK_SIDEB,     ATTACKTYPE_DEFENSIVE, 1);
		addAIAttackType(ATTACK_SIDEB,     ATTACKTYPE_SIDEMOVE, 1);
		addAIAttackType(ATTACK_UPB,       ATTACKTYPE_STRONGMELEE, 1);
		addAIAttackType(ATTACK_UPB,       ATTACKTYPE_UPMOVE, 1);
		addAIAttackType(ATTACK_SIDESMASH, ATTACKTYPE_STRONGMELEE, 2, true);
		addAIAttackType(ATTACK_UPSMASH,   ATTACKTYPE_STRONGMELEE, 1, true);
		addAIAttackType(ATTACK_DOWNSMASH, ATTACKTYPE_STRONGMELEE, 1, true);
		addAIAttackType(ATTACK_FINALSMASH,ATTACKTYPE_PROJECTILE, 1);
	}
	
	void PaperMario::Unload()
	{
		Player::Unload();
	}
	
	void PaperMario::addP2PData(DataVoid&data)
	{
		int total = createdProjectiles.size();
		
		if(total>0)
		{
			P2PDataManager::setReliable();
			
			bool avail = true;
			data.add(&avail, sizeof(avail));
			
			int total = createdProjectiles.size();
			data.add(&total, sizeof(total));
			
			for(int i=0; i<createdProjectiles.size(); i++)
			{
				ProjectileInfo proj = createdProjectiles.get(i);
				
				data.add(&proj.type, sizeof(proj.type));
				data.add(&proj.projID, sizeof(proj.projID));
				data.add(&proj.x, sizeof(proj.x));
				data.add(&proj.y, sizeof(proj.y));
			}
			createdProjectiles.clear();
		}
		else
		{
			bool avail = false;
			data.add(&avail, sizeof(avail));
		}
	}
	
	void PaperMario::setP2PData(byte*&data)
	{
		bool avail = DataVoid::toBool(data);
		data += sizeof(bool);
		
		if(avail)
		{
			int total = DataVoid::toInt(data);
			data += sizeof(int);
			
			for(int i=0; i<total; i++)
			{
				byte type = data[0];
				data += sizeof(byte);
				
				int projID = DataVoid::toInt(data);
				data += sizeof(int);
				
				float x1 = DataVoid::toFloat(data);
				data += sizeof(float);
				float y1 = DataVoid::toFloat(data);
				data += sizeof(float);
				
				switch(type)
				{
					case 1:
					{
						Projectile::setNextID(projID);
						createProjectile(new Hammer(getPlayerNo(), x1, y1));
					}
					break;
					
					case 2:
					{
						Projectile::setNextID(projID);
						createProjectile(new Confetti(getPlayerNo(), x1, y1));
					}
					break;
					
					case 3:
					{
						Projectile::setNextID(projID);
						createProjectile(new FinalSmashOrigami(getPlayerNo(), x1, y1));
					}
					break;
				}
			}
		}
	}
	
	void PaperMario::Update(long gameTime)
	{
		if(chargedDownAttack)
		{
			if(chargeDownTime <= Global::getWorldTime())
			{
				chargedDownAttack = false;
				chargeDownTime = 0;
			}
		}
		Player::Update(gameTime);
		updateGravity();
		updateFrame();
		
		updateMovement();
		
		updateHanging();
		updateHurt();
		updateDrop();
		
		updateAI();
	}
	
	void PaperMario::Draw(Graphics2D&g, long gameTime)
	{
		Player::Draw(g, gameTime);
	}
	
	void PaperMario::onAnimationFinish(const String&n)
	{
		if(n.equals("special_charge_down_left") || n.equals("special_charge_down_right")
		|| n.equals("special_attack_down_left") || n.equals("special_attack_down_right"))
		{
			//
		}
		else if(n.equals("special_attack_up_left") || n.equals("special_attack_up_right"))
		{
			xvelocity=0;
			animFinish();
		}
		else if(n.equals("finalsmash_begin_left") || n.equals("finalsmash_begin_right"))
		{
			// Switch to samurai attack animation
			if(n.equals("finalsmash_begin_left"))
			{
				changeAnimation("finalsmash_samurai_left", FORWARD);
			}
			else
			{
				changeAnimation("finalsmash_samurai_right", FORWARD);
			}
			
			// Create fan projectile attack (origami fans)
			FinalSmashOrigami*fire = new FinalSmashOrigami(getPlayerNo(), x + (60 * getPlayerDirMult()), y-40);
			createProjectile(fire);
			
			addProjectileInfo(3, fire->getID(), x + (60 * getPlayerDirMult()), y-40);
		}
		else if(n.equals("finalsmash_samurai_left") || n.equals("finalsmash_samurai_right"))
		{
			// Samurai attack sequence finished, create additional fan projectiles
			for(int i=0; i<3; i++)
			{
				float offsetX = (80 + i*40) * getPlayerDirMult();
				float offsetY = -20 - i*10;
				FinalSmashOrigami*fire = new FinalSmashOrigami(getPlayerNo(), x + offsetX, y + offsetY);
				createProjectile(fire);
				addProjectileInfo(3, fire->getID(), x + offsetX, y + offsetY);
			}
			changeTwoSidedAnimation("finalsmash_end", FORWARD);
		}
		else if(n.equals("finalsmash_end_left") || n.equals("finalsmash_end_right"))
		{
			animFinish();
			AttackTemplates::finishFinalSmash(this);
		}
		else
		{
			Player::onAnimationFinish(n);
		}
	}
	
	void PaperMario::jump()
	{
		Player::jump(4.9f,5.1f);
	}
	
	void PaperMario::onPlayerHit(Player*collide, byte dir)
	{
		switch(dir)
		{
			case DIR_LEFT:
			case DIR_RIGHT:
			if(compareDirPlayerDir(dir, getPlayerDir())==CMPDIRPDIR_EQUAL)
			{
				switch(attacksHolder)
				{
					case 0:
					//A
					if(hitAmount==0)
					{
						causeDamage(collide,3);
						collide->y -= 3;
						collide->x -= 3 * getPlayerDirMult();
						x += 1 * getPlayerDirMult();
						causeHurt(collide, getOppPlayerDir(), 100);
						hitAmount++;
					}
					break;
					
					case 1:
					//A 2
					if(hitAmount==0)
					{
						causeDamage(collide,2);
						collide->y -= 3;
						collide->x -= 3 * getPlayerDirMult();
						x += 1 * getPlayerDirMult();
						causeHurt(collide, getOppPlayerDir(), 100);
						hitAmount++;
					}
					break;
					
					case 2:
					//A 3
					if(hitAmount==0)
					{
						causeDamage(collide,4);
						causeHurtLaunch(collide, (int)getPlayerDirMult(),2,2.5f, -1,2,3);
						causeHurt(collide, getOppPlayerDir(), 400);
						hitAmount++;
					}
					break;
					
					case 11:
					//B side
					collide->setPlayerDir(collide->getOppPlayerDir());
					collide->stand();
					causeDamage(collide,8);
					break;
					
					case 14:
					//smash left
					causeDamage(collide,smashPower);
					collide->x += getPlayerDirMult()*5;
					causeHurtLaunch(collide, (int)getPlayerDirMult(),3,((float)smashPower/5), -1,3,((float)smashPower/4));
					causeHurt(collide, getOppPlayerDir(), 300);
					break;
					
					case 15:
					//smash up left
					causeDamage(collide,smashPower);
					causeHurtLaunch(collide, (int)getPlayerDirMult(),4,((float)smashPower/5), -1,3,((float)smashPower/5));
					causeHurt(collide, getOppPlayerDir(), 300);
					break;
					
					case 16:
					//smash down left
					causeDamage(collide,smashPower);
					causeHurtLaunch(collide, (int)getPlayerDirMult(),4,((float)smashPower/6), -1,3,((float)smashPower/5));
					causeHurt(collide, getOppPlayerDir(), 300);
					break;
				}
			}
			break;
			
			case DIR_UP:
			{
				switch(attacksHolder)
				{
					case 14:
					//smash
					causeDamage(collide,smashPower);
					causeHurtLaunch(collide,0,0,0, -1,3,((float)smashPower/4));
					causeHurt(collide, collide->getPlayerDir(), 300);
					break;
					
					case 15:
					//smash up
					causeDamage(collide,smashPower);
					causeHurtLaunch(collide,0,0,0, -1,4,((float)smashPower/4));
					causeHurt(collide, collide->getPlayerDir(), 300);
					break;
					
					case 16:
					//smash down left
					causeDamage(collide,smashPower);
					causeHurtLaunch(collide,0,0,0, -1,4.5f,((float)smashPower/4));
					causeHurt(collide, collide->getPlayerDir(), 300);
					break;
				}
			}
			break;
			
			case DIR_DOWN:
			{
				switch(attacksHolder)
				{
					case 5:
					//A down
					causeDamage(collide,5);
					if(collide->y<=y)
					{
						causeHurtLaunch(collide, (int)getPlayerDirMult(),1,1, -1,2.3f,3.1f);
						causeHurt(collide, getOppPlayerDir(), 500);
					}
					else
					{
						causeHurtLaunch(collide,0,0,0, 1,3,3);
						causeHurt(collide, collide->getPlayerDir(), 500);
					}
					break;
					
					case 9:
					//A air down
					collide->y-=5;
					break;
					
					case 16:
					//smash down left
					causeDamage(collide,smashPower);
					causeHurtLaunch(collide,0,0,0, 1,3.9f,((float)smashPower/4));
					causeHurt(collide, collide->getPlayerDir(), 300);
					break;
				}
			}
			break;
		}
	}
	
	void PaperMario::onPlayerRectHit(Player*collide, byte dir)
	{
		if(attacksHolder==12)
		{
			switch(hitAmount)
			{
				case 0:
				if(getRelPlayerDir(collide)!=getPlayerDir())
				{
					return;
				}
				case 1:
				case 2:
				case 3:
				case 4:
				changeTwoSidedAnimation("special_attack_up", FORWARD);
				causeDamage(collide,2);
				collide->y = (y - ((height/2) + (collide->height/2) + 16));
				collide->x = x;
				collide->xvelocity=0;
				collide->yvelocity=0;
				hitAmount += 1;
				yvelocity = -4;
				causeHurt(collide, collide->getPlayerDir(), 0);
				break;
				
				case 5:
				default:
				changeTwoSidedAnimation("special_attack_up", FORWARD);
				causeDamage(collide,2);
				yvelocity = -2;
				attacksHolder = -1;
				hitAmount += 1;
				collide->yvelocity = 0;
				causeHurtLaunch(collide, 0,0,0, -1,2.4f,3);
				causeHurt(collide, collide->getPlayerDir(), 300);
				break;
			}
		}
	}
	
	boolean PaperMario::onDeflectProjectileCollision(Projectile*collide, byte dir)
	{
		byte relDir = getRelPlayerDir(collide);
		if(attacksHolder==11 && (relDir==getPlayerDir() || relDir==0))
		{
			collide->deflect(dir);
			collide->setOwner(this);
			return true;
		}
		return false;
	}
	
	boolean PaperMario::onDeflectItemCollision(Item*collide, byte dir)
	{
		byte relDir = getRelPlayerDir(collide);
		if(attacksHolder==11 && (relDir==getPlayerDir() || relDir==0))
		{
			collide->deflect(dir);
			return true;
		}
		return false;
	}
	
	void PaperMario::onFinishCharge()
	{
		destroyCharge();
		chargeDownTime = Global::getWorldTime() + 10000;
		chargedDownAttack = true;
		charging_attack = 0;
		animFinish();
	}
	
	void PaperMario::doChargingAttack(byte button)
	{
		destroyCharge();
		chargingAttack = false;
		chargedDownAttack = false;
		charging_attack = 0;
		animFinish();
	}
	
	void PaperMario::attackA()
	{
		addAttackInfo(DIR_LEFT, 6, LEFT, 10, 500, -1,1,1.5f, -1,1.1f,2.1f);
		addAttackInfo(DIR_RIGHT,6,RIGHT, 10, 500,  1,1,1.5f, -1,1.1f,2.1f);
		addAttackInfo(DIR_DOWN, 6, LEFT, 10, 400, 0,0,0, 1,3,2);
		addAttackInfo(DIR_DOWN, 6,RIGHT, 10, 400, 0,0,0, 1,3,2);
		
	    if(!checkItemUse())
	    {
	        if(isOnGround())
	        {
	        	AttackTemplates::combo3A(this, 600, 0,1, 1,1, 2,1.8);
	        }
	        else
	        {
	            if(!bUp)
	            {
	                AttackTemplates::normalAirA(this, 6,1.2);
	            }
	        }
	    }
	}
	
	void PaperMario::attackSideA()
	{
		addAttackInfo(DIR_LEFT, 3, LEFT, 8, 500, -1,2,2.6f, -1,1.5f,2.5f);
		addAttackInfo(DIR_RIGHT,3,RIGHT, 8, 500,  1,2,2.6f, -1,1.5f,2.5f);
		addAttackInfo(DIR_DOWN, 3, LEFT, 8, 500, -1,2,2.6f, -1,1.5f,2.5f);
		addAttackInfo(DIR_DOWN, 3,RIGHT, 8, 500,  1,2,2.6f, -1,1.5f,2.5f);
		addAttackInfo(DIR_LEFT, 7, LEFT, 10, 600, -1,2,2.9f, -1,0.5f,0);
		addAttackInfo(DIR_RIGHT,7,RIGHT, 10, 600,  1,2,2.9f, -1,0.5f,0);
		addAttackInfo(DIR_DOWN, 7, LEFT, 10, 400, 0,0,0, 1,6,6);
		addAttackInfo(DIR_DOWN, 7,RIGHT, 10, 400, 0,0,0, 1,6,6);
		
	    if(((getMoveLeft()==2) || (getMoveRight()==2)) && (isOnGround()))
	    {
	        AttackTemplates::normalDashA(this, 3,2.3, 5.0f);
	    }
	    else
	    {
	        if(!checkItemUseSide())
	        {
	            if(isOnGround())
	            {
	            	AttackTemplates::normalSideA(this, 3,2.1);
	            }
	            else
	            {
	                if(!bUp)
	                {
	                	AttackTemplates::normalAirSideA(this, 7,2.8);
	                }
	            }
	        }
	    }
	}
	
	void PaperMario::attackUpA()
	{
		addAttackInfo(DIR_LEFT, 8, LEFT, 10, 600, -1,2,2.7f, -1,1,2.2f);
		addAttackInfo(DIR_RIGHT,8,RIGHT, 10, 600,  1,2,2.7f, -1,1,2.2f);
		addAttackInfo(DIR_UP, 8, LEFT, 10, 600, 0,0,0, -1,2,3.6f);
		addAttackInfo(DIR_UP, 8,RIGHT, 10, 600, 0,0,0, -1,2,3.6f);
		addAttackInfo(DIR_DOWN, 8, LEFT, 10, 600, 0,0,0, 1,4,5);
		addAttackInfo(DIR_DOWN, 8,RIGHT, 10, 600, 0,0,0, 1,4,5);
		addAttackInfo(DIR_UP, 4, LEFT, 7, 400, 0,0,0, -1,2,3);
		addAttackInfo(DIR_UP, 4,RIGHT, 7, 400, 0,0,0, -1,2,3);
		addAttackInfo(DIR_LEFT,  4, LEFT, 7, 400, -1,1,2, -1,1,1.6f);
		addAttackInfo(DIR_RIGHT, 4,RIGHT, 7, 400, 1,1,2, -1,1,1.6f);
		
		if(!checkItemUseUp())
		{
			if(isOnGround())
			{
				AttackTemplates::normalUpA(this, 4, 0.95);
			}
			else
			{
				if(!bUp)
				{
					AttackTemplates::normalAirUpA(this, 8,2.349);
				}
			}
		}
	}
	
	void PaperMario::attackDownA()
	{
		addAttackInfo(DIR_LEFT, 5, LEFT, 5, 500, -1,2,1.8f, -1,2.5f,2);
		addAttackInfo(DIR_RIGHT,5,RIGHT, 5, 500,  1,2,1.8f, -1,2.5f,2);
		addAttackInfo(DIR_UP,   5, LEFT, 5, 500,  0,0,0, -1,2.3f,3.1f);
		addAttackInfo(DIR_UP,   5,RIGHT, 5, 500,  0,0,0, -1,2.3f,3.1f);
		addAttackInfo(DIR_LEFT, 9, LEFT, 7, 600, -1,3,3.1f, -1,1,3);
		addAttackInfo(DIR_LEFT, 9,RIGHT, 7, 600, -1,3,3.1f, -1,1,3);
		addAttackInfo(DIR_RIGHT, 9, LEFT, 7, 600,  1,3,3.1f, -1,1,3);
		addAttackInfo(DIR_RIGHT, 9,RIGHT, 7, 600,  1,3,3.1f, -1,1,3);
		addAttackInfo(DIR_UP, 9, LEFT, 7, 600,  0,0,0, -1,3,2);
		addAttackInfo(DIR_UP, 9,RIGHT, 7, 600,  0,0,0, -1,3,2);
		addAttackInfo(DIR_DOWN, 9, LEFT, 7, 300, 0,0,0, 1,4,5);
		addAttackInfo(DIR_DOWN, 9,RIGHT, 7, 300, 0,0,0, 1,4,5);
		
		if(!checkItemUseDown())
		{
			if(isOnGround())
			{
				AttackTemplates::normalDownA(this, 5,2.15);
			}
			else
			{
				if(!bUp)
				{
					AttackTemplates::singleAirDownA(this, 9,2.65);
				}
			}
		}
	}
	
	void PaperMario::attackB()
	{
	    if(!bUp)
	    {
	        if((canFinalSmash())&&(isOnGround()))
	        {
	            attackFinalSmash();
	        }
	        else
	        {
				Hammer*hammer = new Hammer(this->getPlayerNo(),x,y);
	        	AttackTemplates::singleProjectile(this, 10,0, hammer);
				
				addProjectileInfo(1, hammer->getID(), x, y);
				
				changeTwoSidedAnimation("special_attack",FORWARD);
	        }
	    }
	}
	
	void PaperMario::attackSideB()
	{
	    if(!bUp)
	    {
	        AttackTemplates::moveSideB(this, 11,1.83f, 2.99f);
	        if(!isOnGround())
	        {
	        	xvelocity += 2 * getPlayerDirMult();
	        }
	    }
	}
	
	void PaperMario::attackUpB()
	{
	    if(!bUp)
	    {
	        AttackTemplates::rehitUpB(this, 12, 3.1f, 5.5f, 5.5f);
	        if(isOnGround())
	        {
	        	xvelocity = 0.8f * getPlayerDirMult();
	        }
	        else
	        {
	        	xvelocity = 2.1f * getPlayerDirMult();
	        }
	    }
	}
	
	void PaperMario::attackDownB()
	{
		if(!bUp)
		{
			if(charging_attack==0)
			{
				if(chargedDownAttack)
				{
					Confetti*fludd = new Confetti(getPlayerNo(), x+(getPlayerDirMult()*(66*getScale())), y-(1*getScale()));
					fludd->setScale(getScale());
					AttackTemplates::singleProjectile(this, 13, 0, fludd);
					changeTwoSidedAnimation("special_attack_down", FORWARD);
					chargedDownAttack = false;
				}
				else
				{
					AttackTemplates::chargeB(this, 0, 100, 1000);
					changeTwoSidedAnimation("special_charge_down", FORWARD);
					charging_attack = 1;
				}
			}
		}
	}
	
	void PaperMario::attackSideSmash(byte type)
	{
		if(!bUp && !checkItemUseSideSmash(type))
		{
			if(isOnGround())
			{
				AttackTemplates::normalSmash(this, 14, 3.28, type, 1, 1500, 11, 20);
			}
			else
			{
				attackSideA();
			}
		}
	}
	
	void PaperMario::attackUpSmash(byte type)
	{
		if(!bUp && !checkItemUseUpSmash(type))
		{
			if(isOnGround())
			{
				AttackTemplates::normalSmash(this, 15, 3.26, type, 2, 1500, 10, 20);
			}
			else
			{
				attackUpA();
			}
		}
	}
	
	void PaperMario::attackDownSmash(byte type)
	{
		if(!bUp && !checkItemUseDownSmash(type))
		{
			if(isOnGround())
			{
				AttackTemplates::normalSmash(this, 16, 3.27, type, 3, 1500, 10, 20);
			}
			else
			{
				attackDownA();
			}
		}
	}
	
	void PaperMario::attackFinalSmash()
	{
        addAttackInfo(DIR_LEFT, 17, LEFT, 30, 900, -1,7,4.5f, -1,6,5);
        addAttackInfo(DIR_RIGHT,17,RIGHT, 30, 900,  1,7,4.5f, -1,6,5);
        addAttackInfo(DIR_UP,   17, LEFT, 30, 900, 0,0,0, -1,5,6);
        addAttackInfo(DIR_UP,   17,RIGHT, 30, 900, 0,0,0, -1,5,6);
        addAttackInfo(DIR_DOWN, 17, LEFT, 30, 900, 0,0,0,  1,5,6);
        addAttackInfo(DIR_DOWN, 17,RIGHT, 30, 900, 0,0,0,  1,5,6);
        
	    if(isOnGround())
	    {
	    	AttackTemplates::finalSmash(this, 17);
			changeTwoSidedAnimation("finalsmash_begin", FORWARD);
	    }
	    else
	    {
	        attackB();
	    }
	}
	
	PaperMario::Hammer::Hammer(byte playerNo, float x1, float y1) : Projectile(playerNo,x1,y1)
	{
		xspeed = 3.1f;
		yspeed = 2;
		
		setDeflectable(true);

		Animation*anim;
		anim = new Animation("left", 8, 4, 1);
		anim->addFrame("Images/Game/Characters/PaperMario/fireball.png");
		addAnimation(anim);
		
		anim = new Animation("right", 8, 4, 1);
		anim->addFrame("Images/Game/Characters/PaperMario/fireball.png");
		anim->mirror(true);
		addAnimation(anim);
		
		switch(itemdir)
		{
			case LEFT:
			xvelocity=-xspeed;
			changeAnimation("left",FORWARD);
			break;
				
			case RIGHT:
			xvelocity=xspeed;
			changeAnimation("right",FORWARD);
			break;
		}
		liveTime = Global::getWorldTime() + 1500;
	}
	
	PaperMario::Hammer::~Hammer()
	{
		//
	}
	
	void PaperMario::Hammer::deflect(byte dir)
	{
		switch(dir)
		{
			case DIR_LEFT:
			case DIR_UPLEFT:
			case DIR_DOWNLEFT:
			itemdir = LEFT;
			xvelocity = -xspeed;
			changeAnimation("left", NO_CHANGE);
			break;
			
			case DIR_RIGHT:
			case DIR_UPRIGHT:
			case DIR_DOWNRIGHT:
			itemdir = RIGHT;
			xvelocity = xspeed;
			changeAnimation("right", NO_CHANGE);
			break;
		}
	}
	
	void PaperMario::Hammer::Update(long gameTime)
	{
		Projectile::Update(gameTime);
		yvelocity += 0.2f;
		if(liveTime <= Global::getWorldTime())
		{
			this->destroy();
		}
	}
	
	void PaperMario::Hammer::onPlayerHit(Player*collide, byte dir)
	{
		if(isHittable(collide,dir))
		{
			if(collide->getAttacksPriority()==5)
			{
			    if(collide->getPlayerDir()==1)
			    {
			        xvelocity=-xvelocity;
			        yvelocity=-yvelocity;
			    }
			    else
			    {
			        destroy();
			        causeDamage(collide,5);
			        collide->y-=3;
			        switch(itemdir)
			        {
			            case LEFT:
			            causeHurtLaunch(collide,-1,1,3, -1,1,3);
			            causeHurt(collide, RIGHT, 200);
			            break;
			 
			            case RIGHT:
			            causeHurtLaunch(collide, 1,1,3, -1,1,3);
			            causeHurt(collide, LEFT, 200);
			            break;
			        }
			    }
			}
				
			else if(collide->getAttacksPriority()==6)
			{
			    xvelocity=-xvelocity;
			    yvelocity=-yvelocity;
			}
				
			else if(collide->getAttacksPriority()==7)
			{
			    destroy();
			}
				
			else
			{
			    destroy();
			    causeDamage(collide,5);
			    collide->y-=3;
			    switch(itemdir)
			    {
			        case LEFT:
			        causeHurtLaunch(collide, -1,1,3, -1,1,3);
			        causeHurt(collide, RIGHT, 200);
			        break;
			 
			        case RIGHT:
			        causeHurtLaunch(collide, 1,1,3, -1,1,3);
			        causeHurt(collide, LEFT, 200);
			        break;
			    }
			}
		}
	}
	
	void PaperMario::Hammer::whilePlatformColliding(Platform*collide, byte dir)
	{
		switch(dir)
		{
			case DIR_UP:
			yvelocity=-yspeed;
			break;
				
			case DIR_DOWN:
			yvelocity=yspeed;
			break;
				
			case DIR_LEFT:
			xvelocity=-xspeed;
			break;
				
			case DIR_RIGHT:
			xvelocity=xspeed;
			break;
		}
	}
	
	PaperMario::Confetti::Confetti(byte playerNo, float x1, float y1) : Projectile(playerNo, x1, y1)
	{
		Animation*anim;
			
		anim = new Animation("left",10,10,1);
		anim->addFrame("Images/Game/Characters/PaperMario/fludd_water.png");
		addAnimation(anim);
			
		anim = new Animation("right",10,10,1);
		anim->addFrame("Images/Game/Characters/PaperMario/fludd_water.png");
		anim->mirror(true);
		addAnimation(anim);
			
		switch(itemdir)
		{
			case LEFT:
			changeAnimation("left",FORWARD);
			break;
				
			case RIGHT:
			changeAnimation("right",FORWARD);
			break;
		}
	}
	
	PaperMario::Confetti::~Confetti()
	{
		//
	}

	void PaperMario::Confetti::Update(long gameTime)
	{
		Player* owner = Global::getPlayerActor(getPlayerNo());
		this->x = owner->x+(owner->getPlayerDirMult()*(66*owner->getScale()));
		this->y = owner->y - owner->getScale();
		Projectile::Update(gameTime);
		Player*playr = Global::getPlayerActor(this->getPlayerNo());
		if(!playr->isAlive() && !playr->isHurt())
		{
			destroy();
		}
	}
	
	void PaperMario::Confetti::whilePlayerHitting(Player*collide, byte dir)
	{
		if(collide->x < Global::getPlayerActor(getPlayerNo())->x)
		{
			collide->x -= 2;
		}
		else
		{
			collide->x += 2;
		}
	}
		
	void PaperMario::Confetti::onAnimationFinish(const String&aName)
	{
		destroy();
		PaperMario*playr = (PaperMario*)Global::getPlayerActor(getPlayerNo());
		playr->animFinish();
	}
	
	PaperMario::FinalSmashOrigami::FinalSmashOrigami(byte playerNo, float x1, float y1) : Projectile(playerNo,x1,y1)
	{
		Animation*anim;
		
		anim = new Animation("left", 12, 5, 2);
		anim->addFrame("Images/Game/Characters/PaperMario/finalsmash_fire.png");
		addAnimation(anim);
		
		anim = new Animation("right", 12, 5, 2);
		anim->addFrame("Images/Game/Characters/PaperMario/finalsmash_fire.png");
		anim->mirror(true);
		addAnimation(anim);
		
		switch(itemdir)
		{
			case LEFT:
			changeAnimation("left",FORWARD);
			break;
				
			case RIGHT:
			changeAnimation("right",FORWARD);
			break;
		}
	}

	PaperMario::FinalSmashOrigami::~FinalSmashOrigami()
	{
		//
	}
		
	void PaperMario::FinalSmashOrigami::onPlayerHit(Player*collide, byte dir)
	{
		if(isHittable(collide,dir))
		{
			switch(dir)
			{
				case DIR_UP:
				causeDamage(collide,30);
				causeHurtLaunch(collide, 0,0,0, -1,6,7);
				causeHurt(collide, collide->getPlayerDir(), 900);
				break;
					
				case DIR_DOWN:
				causeDamage(collide,30);
				causeHurtLaunch(collide, 0,0,0, 1,6,7);
				causeHurt(collide, collide->getPlayerDir(), 900);
				break;
					
				case DIR_LEFT:
				causeDamage(collide,30);
				causeHurtLaunch(collide, -1,7,6.5f, -1,6,7);
				causeHurt(collide, RIGHT, 900);
				break;
					
				case DIR_RIGHT:
				causeDamage(collide,30);
				causeHurtLaunch(collide, 1,7,6.5f, -1,6,7);
				causeHurt(collide, LEFT, 900);
				break;
			}
		}
	}
		
	void PaperMario::FinalSmashOrigami::onAnimationFinish(const String&n)
	{
		Player*playr = Global::getPlayerActor(this->getPlayerNo());
		playr->changeTwoSidedAnimation("finalsmash_end", FORWARD);
		destroy();
	}
}
