#ifndef _FREQ_H
#define _FREQ_H

#include <iostream>
#include <chrono>
#include "Util.h"

class Freq
{
public:

    class Time
    {
    public:
        unsigned int value;
        Time():
            value(0) {}
        explicit Time(unsigned int ms) {
            value = ms;
        }
        Time(const Time& t) {
            value = t.internal();
        }
        unsigned int internal() const { return value; }
        //static Time seconds(unsigned int s) { return Time(s * 1000);}
        static Time seconds(float s) { return Time((unsigned int)(s * 1000.0f)); }
        //static Time minutes(unsigned int m) { return Time(m * 60000);}
        static Time minutes(float m) { return Time((unsigned int)(m * 60000.0f));}
        static Time ms(unsigned int ms) { return Time(ms); }

        operator bool() const { return value; }

        Time& operator+=(Time t) {
            value += t.internal();
            return *this;
        }

        float s() const { return value / 1000.0f; }
        float seconds() const { return value / 1000.0f; }
        unsigned int ms() const { return value; }
    };

    class Timeline {
        private:
            //unsigned long m_ulPassedTime;
            unsigned long m_ulPassedTime;
            //unsigned int m_uiLastAdvance;
            float m_fSpeed;
        public:
            Timeline():
                m_ulPassedTime(0L),
                m_fSpeed(1.0f)
            {}
            virtual ~Timeline() {}
            virtual unsigned long ms() const {
                return m_ulPassedTime;
            }
            virtual float seconds() const {
                return m_ulPassedTime / 1000.0f;
            }
            
            /*virtual */Freq::Time logic(Freq::Time t) { // ms
                float advance = t.ms() * m_fSpeed;
                //m_uiLastAdvance = round_int(advance);
                m_ulPassedTime += std::rint(advance);
                return Freq::Time::ms(std::rint(advance));
            }
            //float logic(float a) { // seconds
            //    float advance = a * m_fSpeed;
            //    //m_uiLastAdvance = round_int(advance);
            //    m_ulPassedTime += std::rint(advance * 1000.0f);
            //    return advance;
            //}
            //unsigned int advance() const { return m_uiLastAdvance; }

            void speed(float s) {
                m_fSpeed = s;
            }
            float speed() const {
                return m_fSpeed;
            }
            void pause() {
                m_fSpeed = 0.0f;
            }
            void resume(float speed = 1.0f) {
                m_fSpeed = speed;
            }
            void reset() {
                m_ulPassedTime = 0L;
                m_fSpeed = 1.0f;
            }
    };

    // eventually migrate to chrono
    class Alarm
    {
    protected:
    
        //Freq* m_pTimer;
        Timeline* m_pTimer;
        unsigned long m_ulAlarmTime;
        unsigned long m_ulStartTime;

        //todo:add alarm pausing?
    
    public:
    
        //Alarm():
        //    m_ulAlarmTime(0L),
        //    m_ulStartTime(0L),
        //    m_pTimer(Freq::get().accumulator())
        //{
        //    assert(m_pTimer);
        //}

        explicit Alarm(Timeline* timer):
            m_ulAlarmTime(0L),
            m_ulStartTime(0L),
            m_pTimer(timer)
        {
            assert(m_pTimer);
        }

        explicit Alarm(Time t, Timeline* timer):
            m_pTimer(timer)
            //m_pTimer(timer ? timer : Freq::get().accumulator())
        {
            assert(m_pTimer);
            set(t);
        }

        virtual ~Alarm() {}
        
        Alarm& operator+=(Freq::Time t) {
            m_ulAlarmTime += t.ms();
            return *this;
        }

        bool hasTimer() const { return (m_pTimer!=NULL); }
        
        void timer(Timeline* timerRef)
        {
            assert(timerRef);
            m_pTimer = timerRef;
        }
        const Timeline* timer() const { return m_pTimer; }
        Timeline* timer() { return m_pTimer; }
        
        void set(Time t, Timeline* timer = NULL)
        {
            if(timer)
                m_pTimer = timer;
            assert(m_pTimer);
            m_ulStartTime = m_pTimer->ms();
            m_ulAlarmTime = m_ulStartTime + t.ms();
        }

        void delay(Time t) {
            assert(m_pTimer);
            m_ulAlarmTime += ((unsigned long)t.ms());
        }

        Freq::Time pause() {
            return Freq::Time(m_ulAlarmTime - m_ulStartTime);
        }

        void minutes(unsigned int m)
        {
           set(Time::minutes(m));
        }
        
        void seconds(unsigned int s)
        {
           set(Time::seconds(s));
        }

        void ms(unsigned int ms)
        {
           set(Time(ms));
        }
        
        unsigned long ms() const
        {
            assert(m_pTimer);
            if(!elapsed())
            {
                unsigned long t = (m_ulAlarmTime - m_pTimer->ms());
                return t;
            }
            return 0L;
        }
        
        float seconds() const
        {
            assert(m_pTimer);
            float t = (m_ulAlarmTime - m_pTimer->ms()) / 1000.0f;
            return t;
        }
        
        bool elapsed() const
        {
            assert(m_pTimer);
            return (m_pTimer->ms() >= m_ulAlarmTime);
        }
        
        float fraction() const
        {
            return 1.0f - fraction_left();
        }

        Freq::Time excess() const {
            if(!elapsed())
                return Freq::Time(0);
            return Freq::Time(m_pTimer->ms() - m_ulAlarmTime);
        }

        float fraction_left() const
        {
            if(elapsed())
                return 0.0f;

            unsigned long remaining = ms();
            unsigned long range = m_ulAlarmTime - m_ulStartTime;
            if(range == 0UL)
                return 0.0f;
            return (float)remaining / (float)range;
        }

        //unsigned long endTickTime() { return m_ulAlarmTime; }
    };

    template<class T>
    class Timed : public Freq::Alarm
    {
    protected:
        Time m_Length;
        T m_Start;
        T m_End;
    public:
        //Timed():
        //    Alarm()
        //{
        //    m_Length = Time(0);
        //}
        Timed(Time t, T start, T end) {
            m_Length = Time(t);
            set(t, start, end);
        }
        explicit Timed(Timeline* timer) {
            m_Length = Time(0);
            this->timer(timer);
        }
        //Timed(const Timed<T>& t) {
        //    m_Start = t.start();
        //    m_End = t.end();
        //    m_Length = t.length();
        //}
        virtual ~Timed() {}
        T get() const{
            return m_Start + (m_End - m_Start) * fraction();
        }
        T inverse() const {
            return m_End - (m_End - m_Start) * fraction();
        }
        T start() const{
            return m_Start;
        }
        T end() const {
            return m_End;
        }
        T diff() const {
            return m_End - m_Start;
        }
        void restart() {
            static_cast<Alarm*>(this)->set(m_Length);
        }
        void set(Time t, T start, T end) {
            m_Start = start;
            m_End = end;
            m_Length = Time(t);
            static_cast<Alarm*>(this)->set(t);
        }
        void clear(T val) {
            m_Start = m_End = val;
            m_Length = Time(0);
            static_cast<Alarm*>(this)->set(Time(m_Length));
        }
        void shift(T val){
            //m_Start = m_End = (m_End + val);
            //m_Length = Time(0);
            //static_cast<Alarm*>(this)->set(Time(m_Length));

            m_Start += val;
            m_End += val;
        }
        void finish(){
            m_Start = m_End;
            static_cast<Alarm*>(this)->set(Time(0));
        }
        void reverse(){
            std::swap(m_Start, m_End);
            static_cast<Alarm*>(this)->set(m_Length);
        }
    };

private:

    Timeline m_globalTimeline;
    //unsigned long m_uiLastMark;
    unsigned long m_ulStartTime;
    unsigned int m_uiMinTick;
    
public:

    Freq():
        m_ulStartTime(get_ticks()),
        m_uiMinTick(0)
    {}
    
    unsigned long get_ticks() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
        ).count();
        //return SDL_GetTicks();
    }
    
    // TODO: not yet implemented
    //void pause();
    //void unpause();
    
    //bool tick();

    // accumulates time passed, until enough has passed to advance forward
    // may advance more than 1 frame's worth of time
    // returns # of ms to advance
    Freq::Time tick() {
        unsigned long ticks = get_ticks() - m_ulStartTime;
        unsigned int advance = (unsigned int)(ticks - m_globalTimeline.ms());
        if(advance >= m_uiMinTick) {
            m_globalTimeline.logic(Freq::Time::ms(advance));
            return Freq::Time::ms(advance);
        }
        return Freq::Time::ms(0);
    }

    Timeline* timeline() { return &m_globalTimeline; }
};

#endif
