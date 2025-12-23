# Справочник команд DBMS

В этом файле перечислены все поддерживаемые команды базы данных с примерами использования.

## Общие команды

| Команда | Описание | Пример |
| :--- | :--- | :--- |
| `PRINT` | Вывести содержимое всей базы данных на экран. | `PRINT` |
| `EXIT` | Сохранить базу данных в файл и выйти из программы. | `EXIT` |

## Массивы (Array)

| Команда | Описание | Пример |
| :--- | :--- | :--- |
| `MPUSH <name> <val1> [val2...]` | Создать массив (если нет) и добавить элементы в конец. | `MPUSH arr1 10 20 30` |
| `MGET <name> <index>` | Получить элемент по индексу. | `MGET arr1 0` |
| `MDEL <name> <index>` | Удалить элемент по индексу. | `MDEL arr1 1` |
| `MINSERT <name> <index> <val>` | Вставить элемент по индексу. | `MINSERT arr1 0 5` |
| `MSET <name> <index> <val>` | Заменить значение элемента по индексу. | `MSET arr1 0 100` |

## Стеки (Stack)

| Команда | Описание | Пример |
| :--- | :--- | :--- |
| `SPUSH <name> <val>` | Добавить элемент на вершину стека. | `SPUSH s1 hello` |
| `SPOP <name>` | Удалить и вернуть элемент с вершины стека. | `SPOP s1` |
| `SEMPTY <name>` | Проверить, пуст ли стек (TRUE/FALSE). | `SEMPTY s1` |

## Очереди (Queue)

| Команда | Описание | Пример |
| :--- | :--- | :--- |
| `QPUSH <name> <val>` | Добавить элемент в конец очереди. | `QPUSH q1 world` |
| `QPOP <name>` | Удалить и вернуть элемент из начала очереди. | `QPOP q1` |
| `QEMPTY <name>` | Проверить, пуста ли очередь (TRUE/FALSE). | `QEMPTY q1` |

## Односвязные списки (SinglyLinkedList)

| Команда | Описание | Пример |
| :--- | :--- | :--- |
| `LPUSHFRONT <name> <val>` | Добавить элемент в начало списка. | `LPUSHFRONT l1 A` |
| `LPUSHBACK <name> <val>` | Добавить элемент в конец списка. | `LPUSHBACK l1 B` |
| `LPOPFRONT <name>` | Удалить и вернуть элемент из начала. | `LPOPFRONT l1` |
| `LPOPBACK <name>` | Удалить и вернуть элемент из конца. | `LPOPBACK l1` |
| `LREMOVE <name> <val>` | Удалить первое вхождение значения. | `LREMOVE l1 A` |
| `LFIND <name> <val>` | Найти значение в списке (TRUE/FALSE). | `LFIND l1 A` |
| `LINSERT_AFTER <name> <target> <new>` | Вставить `new` после `target`. | `LINSERT_AFTER l1 A C` |
| `LINSERT_BEFORE <name> <target> <new>` | Вставить `new` перед `target`. | `LINSERT_BEFORE l1 A D` |
| `LREMOVE_AFTER <name> <val>` | Удалить элемент после указанного значения. | `LREMOVE_AFTER l1 A` |

## Двусвязные списки (DoublyLinkedList)

| Команда | Описание | Пример |
| :--- | :--- | :--- |
| `DLPUSHFRONT <name> <val>` | Добавить элемент в начало. | `DLPUSHFRONT dl1 A` |
| `DLPUSHBACK <name> <val>` | Добавить элемент в конец. | `DLPUSHBACK dl1 B` |
| `DLPOPFRONT <name>` | Удалить и вернуть элемент из начала. | `DLPOPFRONT dl1` |
| `DLPOPBACK <name>` | Удалить и вернуть элемент из конца. | `DLPOPBACK dl1` |
| `DLREMOVE <name> <val>` | Удалить первое вхождение значения. | `DLREMOVE dl1 A` |
| `DLFIND <name> <val>` | Найти значение (TRUE/FALSE). | `DLFIND dl1 A` |
| `DLINSERT_AFTER <name> <target> <new>` | Вставить `new` после `target`. | `DLINSERT_AFTER dl1 A C` |
| `DLINSERT_BEFORE <name> <target> <new>` | Вставить `new` перед `target`. | `DLINSERT_BEFORE dl1 A D` |
| `DLREMOVE_AFTER <name> <val>` | Удалить элемент после указанного. | `DLREMOVE_AFTER dl1 A` |
| `DLREMOVE_BEFORE <name> <val>` | Удалить элемент перед указанным. | `DLREMOVE_BEFORE dl1 B` |

## Хеш-таблицы (Метод цепочек - HashTableChaining)

| Команда | Описание | Пример |
| :--- | :--- | :--- |
| `HPUT <name> <key> <val>` | Добавить или обновить пару ключ-значение. | `HPUT ht1 user1 John` |
| `HGET <name> <key>` | Получить значение по ключу. | `HGET ht1 user1` |
| `HDEL <name> <key>` | Удалить пару по ключу. | `HDEL ht1 user1` |

## Хеш-таблицы (Открытая адресация - HashTableOpenAddr)

| Команда | Описание | Пример |
| :--- | :--- | :--- |
| `OHPUT <name> <key> <val>` | Добавить или обновить пару ключ-значение. | `OHPUT oh1 user2 Jane` |
| `OHGET <name> <key>` | Получить значение по ключу. | `OHGET oh1 user2` |
| `OHDEL <name> <key>` | Удалить пару по ключу. | `OHDEL oh1 user2` |

## Бинарные деревья (FullBinaryTree)

| Команда | Описание | Пример |
| :--- | :--- | :--- |
| `TINSERT <name> <val>` | Вставить значение в дерево (поуровнево). | `TINSERT tree1 root` |
| `TFIND <name> <val>` | Найти значение в дереве (TRUE/FALSE). | `TFIND tree1 root` |
| `TISFULL <name>` | Проверить, является ли дерево полным (TRUE/FALSE). | `TISFULL tree1` |

## Множества (Set)

| Команда | Описание | Пример |
| :--- | :--- | :--- |
| `SADD <name> <val>` | Добавить элемент в множество. | `SADD set1 apple` |
| `SREM <name> <val>` | Удалить элемент из множества. | `SREM set1 apple` |
| `SISMEMBER <name> <val>` | Проверить наличие элемента (TRUE/FALSE). | `SISMEMBER set1 apple` |

## LFU Кеш (LFUCache)

| Команда | Описание | Пример |
| :--- | :--- | :--- |
| `CPUT <name> <key> <val>` | Добавить значение в кеш (с вытеснением LFU). | `CPUT cache1 k1 v1` |
| `CGET <name> <key>` | Получить значение (увеличивает частоту). | `CGET cache1 k1` |

## Специальные задачи (Tasks)

| Команда | Описание | Пример |
| :--- | :--- | :--- |
| `ASTEROIDS <array_name>` | Решить задачу столкновения астероидов. | `ASTEROIDS arr1` |
| `MINPARTITION <input_set> <out1> <out2>` | Разбить множество на два с мин. разницей сумм. | `MINPARTITION set1 sA sB` |
| `FINDSUM <input_arr> <target> <out_arr>` | Найти два числа в массиве, дающих сумму `target`. | `FINDSUM arr1 10 res` |
| `LONGESTSUBSTR <string>` | Найти длину самой длинной подстроки без повторов. | `LONGESTSUBSTR abcabcbb` |
