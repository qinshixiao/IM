#ifndef MSG
#define MSG

struct Msg
{
    int to_user_id;//if user_id is 0,perform all user
    int from_user_id;
    char from_user_name[20];
    char msg[256];//msg text
};

#endif // MSG

