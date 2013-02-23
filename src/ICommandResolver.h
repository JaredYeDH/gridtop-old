#ifndef _ICOMMANDRESOLVER_H
#define _ICOMMANDRESOLVER_H

class ICommandResolver
{
    public:
        virtual ~ICommandResolver() {}

        virtual unsigned resolve(
            const std::tuple<WindowManager*,
            std::string>& args
        ) const = 0;

};

#endif

