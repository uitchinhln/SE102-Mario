#pragma once
#include "MarioPowerUp.h"

class AttackablePower :
    public MarioPowerUp
{
public:
    AttackablePower(shared_ptr<Mario> mario);

    virtual void AttackUpdate();

    virtual void OnAttackStart() = 0;

    virtual void OnAttackFinish() = 0;

    virtual void InAttackProgress() = 0;

    virtual void MoveUpdate() override;

    virtual void OnAttackDetroyed();

    virtual bool CanAttack();

    virtual bool IsAttacking();

    virtual void Update(vector<shared_ptr<IColliable>>* coObj) override;

    virtual void Render() override;

protected:
    Stopwatch attackTimer;

    DWORD MARIO_ATTACK_TIME = 250;
};

