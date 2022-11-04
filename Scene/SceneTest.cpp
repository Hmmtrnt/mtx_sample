#include "DxLib.h"
#include "game.h"
#include "SceneTest.h"
#include "Pad.h"

namespace
{

	// �n��
	const Vec2 kFieldPos0{ 64.0f, 480.0f };
	const Vec2 kFieldPos1{ 640.0f, 333.0f };
	const Vec2 kFieldPos2{ 540.0f, 0.0f };
	const Vec2 kFieldPos3{ 0.0f, 64.0f };
}

SceneTest::SceneTest():
	m_objPos(),
	m_objVec()
{

}

void SceneTest::init()
{
	m_objPos.x = 320.0f;
	m_objPos.y = 240.0f;

	m_objVec.x = 4.0f;
	m_objVec.y = 4.0f;
}

void SceneTest::end()
{
}

SceneBase* SceneTest::update()
{
	m_objPos += m_objVec;

	// 4�ӂƂ̓�����
	if ((m_objPos.x < 0.0f) && (m_objVec.x < 0.0f))
	{
		m_objPos.x = 0.0f;
		m_objVec.x *= -1.0f;
	}
	if ((m_objPos.x > Game::kScreenWidth) && (m_objVec.x > 0.0f))
	{
		m_objPos.x = Game::kScreenWidth;
		m_objVec.x *= -1.0f;
	}
	if ((m_objPos.y < 0.0f) && (m_objVec.y < 0.0f))
	{
		m_objPos.y = 0.0f;
		m_objVec.y *= -1.0f;
	}
	if ((m_objPos.y > Game::kScreenHeight) && (m_objVec.y > 0.0f))
	{
		m_objPos.y = Game::kScreenHeight;
		m_objVec.y *= -1.0f;
	}

#if true
	calcLine(kFieldPos0, kFieldPos1);
	calcLine(kFieldPos1, kFieldPos2);
	calcLine(kFieldPos2, kFieldPos3);
	calcLine(kFieldPos3, kFieldPos0);
#endif
	return this;
}

void SceneTest::draw()
{
	DrawString(0, 0, "test", GetColor(255, 255, 255));

	DrawCircleAA(m_objPos.x, m_objPos.y, 2.0f, 16, GetColor(255, 255, 255));

	DrawLineAA(kFieldPos0.x, kFieldPos0.y, kFieldPos1.x, kFieldPos1.y, GetColor(255, 255, 255));

	DrawLineAA(kFieldPos1.x, kFieldPos1.y, kFieldPos2.x, kFieldPos2.y, GetColor(255, 255, 255));
	DrawLineAA(kFieldPos2.x, kFieldPos2.y, kFieldPos3.x, kFieldPos3.y, GetColor(255, 255, 255));
	DrawLineAA(kFieldPos3.x, kFieldPos3.y, kFieldPos0.x, kFieldPos0.y, GetColor(255, 255, 255));
}


void SceneTest::calcLine(Vec2 lineStart, Vec2 lineEnd)
{
	// Vec2��2������VECTOR�ɕϊ�
	VECTOR start;
	start.x = lineStart.x;
	start.y = lineStart.y;
	start.z = 0.0f;
	VECTOR end;
	end.x = lineEnd.x;
	end.y = lineEnd.y;
	end.z = 0.0f;

	// �^�������ɕϊ�����s��Ƃ��̋t�s����쐬����
	VECTOR field = VSub(end, start);
	VECTOR normalField;
	normalField.x = Game::kScreenWidth;
	normalField.y = 0.0f;
	normalField.z = 0.0f;
	MATRIX mtx = MGetRotVec2(field, normalField);	// ��]�s��̐���
	MATRIX invMtx = MInverse(mtx);					// �t�s��̐���

	// obj�����������ɍs���Ă��Ȃ����m�F����
	VECTOR toObj;
	toObj.x = m_objPos.x - lineStart.x;
	toObj.y = m_objPos.y - lineStart.y;
	toObj.z = 0.0f;
	VECTOR toObj2 = VTransform(toObj, mtx);

	// �n�ʂƓ������Ă��Ȃ�
	if (toObj2.y < 0.0f)	return;

	// �n�ʂɓ������Ă���

	// �n�ʂ���ɗ���悤�ɂ���
	toObj2.y = 0.0f;
	toObj = VTransform(toObj2, invMtx);
	m_objPos.x = toObj.x + lineStart.x;
	m_objPos.y = toObj.y + lineStart.y;

	// ���ˏ���
	VECTOR objVec;
	objVec.x = m_objVec.x;
	objVec.y = m_objVec.y;
	objVec.z = 0.0f;
	VECTOR objVec2 = VTransform(objVec, mtx);
	objVec2.y *= -1.0f;

	objVec = VTransform(objVec2, invMtx);

	m_objVec.x = objVec.x;
	m_objVec.y = objVec.y;
}
