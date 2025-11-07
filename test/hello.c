
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h> // Потрібен для параметрів
#include <linux/ktime.h>     // Потрібен для ktime_get()
#include <linux/list.h>      // Потрібен для списків
#include <linux/slab.h>      // Потрібен для kmalloc() та kfree()
#include <linux/errno.h>     // Потрібен для -EINVAL

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training (Lab 4)");
MODULE_LICENSE("Dual BSD/GPL");

/*
 * Завдання II: Додавання параметра hello_count
 */
static uint hello_count = 1; // Значення за умовчанням - 1

// Опис макросів для параметра
module_param(hello_count, uint, S_IRUGO);
MODULE_PARM_DESC(hello_count, "Кількість разів для друку 'Hello, world!'");

/*
 * Завдання III: Оголошення структури даних
 */
struct hello_entry {
	struct list_head list; // Елемент struct list_head
	ktime_t timestamp;     // Поле типу ktime_t
};

/*
 * Завдання IV: Створення статичної змінної голови списку
 * Використовуємо макрос LIST_HEAD для ініціалізації
 */
static LIST_HEAD(hello_list);

/*
 * Функція ініціалізації модуля
 */
static int __init hello_init(void)
{
	int i;
	struct hello_entry *entry; // Вказівник для нового елемента списку

	// Використовуємо pr_emerg замість printk(KERN_EMERG ...) для кращого стилю
	pr_emerg("Module init started.\n");

	/*
	 * Завдання II.B та II.C: Перевірка значень параметра
	 */
	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10)) {
		pr_warn("Warning: hello_count is %u (0 or 5-10). Continuing.\n",
			hello_count);
	} else if (hello_count > 10) {
		pr_err("Error: hello_count (%u) is > 10. Aborting load.\n",
		       hello_count);
		return -EINVAL; // Повертаємо помилку, модуль не завантажиться
	}
	// Якщо 1-4, просто продовжуємо

	for (i = 0; i < hello_count; i++) {
		/*
		 * Завдання V: Виділення пам'яті, отримання часу, додавання до списку
		 */

		// Виділяємо пам'ять
		entry = kmalloc(sizeof(*entry), GFP_KERNEL);
		if (!entry) {
			// Якщо kmalloc не вдався, це серйозна помилка
			pr_err("Failed to allocate memory for list entry\n");
			// (Тут треба було б очистити список, але для лаби припустимо)
			return -ENOMEM;
		}

		// Заносимо поточний час
		entry->timestamp = ktime_get();

		// Додаємо новий елемент в кінець списку
		list_add_tail(&entry->list, &hello_list);

		// Друкуємо привітання
		pr_emerg("Hello, world!\n");
	}

	return 0; // Успішне завантаження
}

/*
 * Функція виходу модуля
 */
static void __exit hello_exit(void)
{
	struct hello_entry *entry, *next;

	pr_emerg("Module exit started.\n");

	/*
	 * Завдання VI: Прохід по списку, друк часу, звільнення пам'яті
	 */

	// Використовуємо list_for_each_entry_safe для безпечного вилучення
	list_for_each_entry_safe (entry, next, &hello_list, list) {
		// Друк часу в наносекундах
		pr_info("Event time: %lld ns\n", ktime_to_ns(entry->timestamp));

		// Вилучення елемента зі списку
		list_del(&entry->list);

		// Звільнення пам'яті
		kfree(entry);
	}

	pr_emerg("Goodbye, world! List cleaned.\n");
}

module_init(hello_init);
module_exit(hello_exit);
