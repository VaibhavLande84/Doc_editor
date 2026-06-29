#ifndef USER_FACTORY_H
#define USER_FACTORY_H

#include <string>

#include "../models/user.h"

class UserFactory {
public:
	static User createUser(int userId,
						   const std::string& name,
						   const std::string& dateOfJoining);
};

inline User UserFactory::createUser(int userId,
									const std::string& name,
									const std::string& dateOfJoining)
{
	return User(userId, name, dateOfJoining);
}

#endif // USER_FACTORY_H
/*hae to add saving password ans stuff*/