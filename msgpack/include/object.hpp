#ifndef ARK_MSGPACK_OBJ_HPP
#define ARK_MSGPACK_OBJ_HPP

#include <variant>
/*
#include <iostream>
#include <sstream>
#include <msgpack.hpp>
*/
#include <Ark/Module.hpp>

namespace ValueManager
{
    struct Child
    {
        Child(size_t typeSize) : m_obj(malloc(typeSize))
        {}

        void* getChild()
        {
            return m_obj;
        }

        ~Child()
        {
            free(m_obj);
        }

    private:
        void* m_obj;
    };

    class Container
    {
    public:
        Container()
        {}

        template <typename T>
        void add()
        {
            m_container.push_back(std::make_unique<Child>(sizeof(T)));
            //(*static_cast<T*>(getBackObject())) = T();
        }

        Child getBackChild()
        {
            return *(m_container.back());
        }

        void* getBackObject()
        {
            return (m_container.back().get())->getChild();
        }

        template<typename T>
        std::list<T> getList()
        {
            return m_container;
        }

    //private:
        std::list<std::unique_ptr<Child>> m_container;
    };

    static Container container;

    template <typename T>
    void newObject()
    {
        container.add<T>();
    }
}

using CObject = std::variant<bool, double, std::string, std::vector<Value>>;
extern CObject get_cobject(const Value& ark_object, ValueType type);

extern Value msgpack_obj_handle(std::vector<Value>& args, VM* vm);
extern Value msgpack_sbuffer(std::vector<Value>& args, VM* vm);
extern Value msgpack_obj(std::vector<Value>& args, VM* vm);

#endif