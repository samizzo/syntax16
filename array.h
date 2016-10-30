#ifndef _array_h
#define _array_h

template<typename T, int SIZE> class Array
{
    public:
        Array()
        {
            m_size = 0;
        }

        void push_back(T obj)
        {
            m_data[m_size++] = obj;
        }

        int length()
        {
            return m_size;
        }

        T operator [] (const int index)
        {
            return m_data[index];
        }

    private:
        int m_size;
        T m_data[SIZE];
};

#endif
