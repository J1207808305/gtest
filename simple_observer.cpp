#include <vector>
#include <iostream>

/* 抽象层code */

template<typename CallbackParam>
class slotbase
{
public:
    virtual void execute(CallbackParam para)=0;
};

template<typename CallbackObj,typename CallbackParam>
class slotimpl : public slotbase<CallbackParam>
{
public:
    slotimpl(CallbackObj* pObj,void (CallbackObj::*pMemberFunc)(CallbackParam))
    {
        m_pObj=pObj;
        m_pMemberFunc=pMemberFunc;
    }
    virtual void execute(CallbackParam para)
    {
        (m_pObj->*m_pMemberFunc)(para);
    }
private:
    CallbackObj* m_pObj;
    void (CallbackObj::*m_pMemberFunc)(CallbackParam);
};

template<typename CallbackParam>
class slot 
{
public:
    template<typename CallbackObj>
        slot(CallbackObj* pObj,void (CallbackObj::*pMemberFunc)(CallbackParam)) 
    {
        m_pSlotbase=new slotimpl<CallbackObj,CallbackParam>(pObj,pMemberFunc);
    }
    ~slot()
    {
        delete m_pSlotbase;
    }
    void execute(CallbackParam para)
    {
        m_pSlotbase->execute(para);
    }
private:
    slotbase<CallbackParam>* m_pSlotbase;
};

template<typename CallbackParam>
class signal
{
public:
    template<typename CallbackObj>
    void bind(CallbackObj* pObj,void (CallbackObj::*pMemberFunc)(CallbackParam para))
    {
        m_slots.push_back(new slot<CallbackParam>(pObj,pMemberFunc));
    }
    ~signal()
    {
        typename std::vector<slot<CallbackParam>* >::iterator ite=m_slots.begin();
        for (;ite!=m_slots.end();ite++)
        {
            delete *ite;
        }
    }
    void operator()(CallbackParam para)
    {
        typename std::vector<slot<CallbackParam>* >::iterator ite=m_slots.begin();
        for (;ite!=m_slots.end();ite++)
        {
            (*ite)->execute(para);
        }
    }
    
private:
    std::vector<slot<CallbackParam>* > m_slots;
};

#define CONNECT(sender,signal,receiver,slot)  sender.signal.bind(receiver,slot)

template<typename MsgsType>
class sender
{
public:
    signal<MsgsType> m_valueChanged;

    MsgsType get_value(){return m_value;}

    void set_value(MsgsType new_value)
    {
        this->m_value = new_value;
    }

    void register_event(MsgsType new_value){m_valueChanged(new_value);}

private:
    MsgsType m_value;
};

/* 实现层 code */

struct param_type{
    int a;
    int b;
};

class observer: public sender<struct param_type>
{
public:
    void set_value(struct param_type para)
    {
        if (para.a != this->get_value().a) {
            m_valueChanged(para);
        }

        if (para.b != this->get_value().b) {
            m_valueChanged(para);
        }   
    }
};

class receiver
{
public:
    void callback1(struct param_type para) 
    {
        std::cout<<"receiver1: "<< para.a <<" "<< para.b <<std::endl;
    }
};

class receiver2
{
public:
    void callback2(struct param_type para)
    {
        std::cout<<"receiver2: "<< para.a <<" "<< para.b <<std::endl;
    }
};

int main(int argc,char** arg)
{
    struct param_type param;
    param.a = 1;
    param.b = 1;
    receiver r;
    receiver2 r2;
    observer s;
    CONNECT(s,m_valueChanged,&r,&receiver::callback1);
    CONNECT(s,m_valueChanged,&r2,&receiver2::callback2);
    s.set_value(param);
    return 0;
}