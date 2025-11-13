#include "../include/PlayerLife.h"

void PlayerLife::sendSuicide()
{
	MonoMethod* method = Method::GetMethod("PlayerLife", "sendSuicide", 0);

	if (!method) {
		return;
	}

	Method::Invoke(method, (MonoObject*)this, nullptr, nullptr);
	return;
}
