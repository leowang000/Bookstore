#ifndef BOOKSTORE_2023_TYPE_DEFINITION_H
#define BOOKSTORE_2023_TYPE_DEFINITION_H

#include "Tools/String.h"

using user_id_t = String<30>;
using password_t = String<30>;
using username_t = String<30>;
using privilege_t = String<1>;
using ISBN_t = String<20>;
using book_name_t = String<60>;
using author_t = String<60>;
using keyword_t = String<60>;
using quantity_t = String<16>;
using quantity_input_t = String<10>;
using price_t = String<13>;
using user_t = String<65>;
using count_t = String<10>;
using instruction_t = String<280>;
using log_info_t = String<350>;
using system_time_t = String<6>;

#endif //BOOKSTORE_2023_TYPE_DEFINITION_H