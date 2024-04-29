/********************************************************
* Filename		: thDefTower.h
* Creator		: ac
* Date time		: 2024.04.21
* Description	: def tower header.
********************************************************/


#ifndef __THDEFTOWER_H__
#define __THDEFTOWER_H__

#include "thPlayer.h"
#include "thBase.h"


class CThDefTowerWarrior :
	public CThBaseCharacter
{
public:
	CThDefTowerWarrior();
	~CThDefTowerWarrior();

	thBool init(
		const CHARACTER_DESC_PTR cptSpDesc,
		const short csSpArrVacantPos,
		const float cfWarriorsBirthAngle,
		const float cfWarriorsBirthX, 
		const float cfWarriorsBirthY,
		const short csActionRadius,
		const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo
	);
	thBool initWarriors(const CHARACTER_DESC_PTR cptSpDesc, const short csSpArrVacantPos, const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo);
	void uninit();

	const float getWarriorBirthMoveAngle() const;
	virtual void getCharaterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet);
	virtual void getCharaterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet);

	void setWarriorBirthMoveAngle(const float fAngle);

	virtual void onMouseUp(EventMouse* pEvent);
	virtual void onMouseDown(EventMouse* pEvent);
	virtual void onMouseMove(EventMouse* pEvent);

	virtual thBool globalMonitoring();


private:
	float m_fWarriorBirthMoveAngle;
	float m_fWarriorBirthX;
	float m_fWarriorBirthY;
	short m_sActionRadius;
	CHARACTER_FRAMEINFO_PTR m_ptWarriorFrameInfo;
};
typedef CThDefTowerWarrior* CThDefTowerWarrior_ptr;


class CThDefTower:
	public CThBaseCharacter
{
public:
	CThDefTower();
	~CThDefTower();

	thBool init(const CHARACTER_DESC_PTR pDesc, CHARACTER_ANI_DESC_PTR* arrpAniDesc, DEFTOWER_DESC_PTR ptTowerDesc, const CHARACTER_DESC_PTR* arrpTowerWarriorsDesc, enum THEM_CHARACTER_LEVEL emLevel, const short csTowerWarriosSize);
	thBool initCharaterAnimate(CHARACTER_ANI_DESC_PTR pAniDesc, const int cnGroupPos);
	thBool initBaiscAnimate(CHARACTER_ANI_DESC_PTR* arrpAniDesc);
	thBool initWarriors(const short csCnt, short sSpArrVacantPos);
	thBool initDefTowerWarriorsDesc(const CHARACTER_DESC_PTR* arrpTowerWarriorsDesc, enum THEM_CHARACTER_LEVEL emLevel, const short csSize);
	virtual void uninit();

	virtual void getCharaterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet);
	virtual void getCharaterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet);
	void getAniTagByDesc(const char* cszpDesc, int* pnRet);
	void getAniFrameInfoByTag(const char* cszpTag, CHARACTER_ANI_FRAMEINFO_PTR* ppRet);
	void getWarriorExistsByAngle(const float cfAngle, thBool* pbRet);

	void setWarriorExistsByAngle(const float cfAngle, const short csTag);
	thBool setPlayAniTowerSummon(const short* arrnCondAniTag, const short cnSize, const thBool bIsSummoning);

	virtual void onMouseUp(EventMouse* pEvent);
	virtual void onMouseDown(EventMouse* pEvent);
	virtual void onMouseMove(EventMouse* pEvent);

	virtual void update(float dt);
	virtual thBool globalMonitoring();
	thBool globalMonitoringWarriors();

private:
	void _getSpArrayVacantPos(short* psRet);
	void _getWarArrayVacantPos(short* psRet);
	thBool _setPlayAniOpenTheDoor();
	thBool _setPlayAniCloseTheDoor();
	thBool _setPlayAniWarriorsDie();

	thBool _monitoringWarriorsHealthy(CThDefTowerWarrior_ptr pSp);

private:
	short							m_sVacantPos;
	double							m_dLastSummonWarriors;
	CHARACTER_ANI_TAG				m_tAniTag;
	CHARACTER_FRAMEINFO_PTR			m_pTower;
	CHARACTER_FRAMEINFO_PTR*		m_arrpSpGroup;
	CHARACTER_ANI_FRAMEINFO_PTR*	m_arrpAniGroup;
	DEFTOWER_DESC_PTR				m_ptTowerStatus;
	/* ������սʿ����������ÿ���ȼ� 3 ������. */
	CHARACTER_DESC_PTR				m_arrpWarriorsDesc[THEM_CHARACTER_LEVEL::CHARACTER_MAXLEVEL][THMAX_DEFTOWER_TARLEVEL_WARRIORS];
	CThDefTowerWarrior_ptr*			m_arrpWarriors;
};



#endif // !__THDEFTOWER_H__
