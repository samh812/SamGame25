#ifndef MONEYBAG_H
#define MONEYBAG_H

#include "entity.h"


class MoneyBag : public Entity
{
public:
    MoneyBag();
    virtual ~MoneyBag();

    void SetValue(int value);
    int GetValue() const;

    void Activate(Vector2 position);
    void Deactivate();
    bool IsActive() const;
    void SetSprite(Sprite* pSprite);

private:
    int m_value;
    bool m_active;
};

#endif // MONEYBAG_H