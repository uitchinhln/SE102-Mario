#pragma once
#include "MarioPower.h"
#include "StopWatch.h"

class AttackablePower :
    public MarioPower
{
public:
    AttackablePower(shared_ptr<Mario> mario);

    virtual void AttackUpdate();

    virtual void OnAttackStart() = 0;

    virtual void OnAttackFinish() = 0;

    virtual void InAttackProgress() = 0;

    virtual void MoveProcess() override;

    virtual void OnAttackDetroyed();

    virtual bool CanAttack();

    virtual bool IsAttacking();

    virtual void Update() override;

    virtual void Render() override;

protected:
    Stopwatch attackTimer;

    long MARIO_ATTACK_DURATION = 250;
};

