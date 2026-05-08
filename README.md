# GAS_RepGraph

Сетевой прототип на Unreal Engine 5 с интеграцией Gameplay Ability System (GAS).  
Проект построен на шаблоне FPS и расширен архитектурой, где состояние способностей и атрибутов хранится в `PlayerState`, а параметры классов задаются через `DataAsset`.

## Что реализовано

- Полноценная базовая интеграция GAS (`GameplayAbilities`, `GameplayTags`, `GameplayTasks`)
- Разделение ответственности между `Character`, `PlayerState` и `AbilitySystemComponent`
- Инициализация GAS и на сервере, и на клиенте для корректной сетевой работы
- Конфигурация стартовых атрибутов и способностей через данные (`UCharacterClassInfo`)
- Репликация атрибутов (Health/Mana/Stamina) и обработка их изменений
- Базовые FPS-механики: движение, обзор, прыжок, оружие/снаряд, pick-up
- Серверный актор эффектов (`AEffectActor`) с наложением `GameplayEffect` по overlap

## Архитектура

### `AGAS_RepGraphCharacter`

- Реализует `IAbilitySystemInterface`
- Отвечает за ввод, камеру и базовое управление персонажем
- Получает `ASC` и `AttributeSet` из `PlayerState`
- Инициализирует GAS:
  - на сервере в `PossessedBy`
  - на клиенте в `OnRep_PlayerState`
- Подписывается на изменения атрибутов (`Health`, `Mana`) и пробрасывает их в Blueprint-события

### `AGASPlayerState`

- Является владельцем:
  - `UGASAbilitySystemComponent`
  - `UGASAttributeSet`
- Использует `EGameplayEffectReplicationMode::Mixed`
- Повышает `NetUpdateFrequency` для более отзывчивого обновления атрибутов
- Такой подход сохраняет состояние способностей/эффектов между респавнами персонажа

### `UGASAbilitySystemComponent`

Кастомный ASC с функциями:

- `AddCharacterAbilities(...)` - выдача активных способностей
- `AddCharacterPassiveAbilities(...)` - выдача и мгновенная активация пассивных способностей
- `InitializeDefaultAttributes(...)` - применение эффекта инициализации атрибутов

### `UGASAttributeSet`

- Реплицируемые атрибуты:
  - `Health`, `MaxHealth`
  - `Mana`, `MaxMana`
  - `Stamina`, `MaxStamina`
- Кламп значений в `PostGameplayEffectExecute` (например, `Health` в диапазоне `0..MaxHealth`)
- `OnRep_*` функции для корректных клиентских уведомлений GAS

### Data-driven конфиг классов

- `UCharacterClassInfo` (`UDataAsset`) хранит `TMap<FGameplayTag, FCharacterClassDefaultInfo>`
- Для каждого класса можно задать:
  - `DefaultAttributes` (`GameplayEffect`)
  - `StartingAbilities`
  - `StartingPassives`
- Персонаж выбирает конфиг по `CharacterTag` и применяет его при инициализации

### `AGAS_RepGraphGameMode` и `UGASAbilitySystemLibrary`

- `GameMode` хранит ссылку на `ClassDefaults` (`UCharacterClassInfo`)
- `UGASAbilitySystemLibrary::GetCharacterClassDefaultInfo(...)` предоставляет доступ к данным класса через мир/контекст
- Логика корректно учитывает, что `GameMode` существует только на сервере

## Сетевой поток инициализации GAS

1. Игрок получает `Controller`
2. На сервере вызывается `PossessedBy`
3. Персонаж получает `ASC/AttributeSet` из `PlayerState`
4. `ASC->InitAbilityActorInfo(PlayerState, Character)`
5. На сервере применяются class defaults (атрибуты, активные и пассивные способности)
6. На клиентах `OnRep_PlayerState` повторно инициализирует `AbilityActorInfo`

## Дополнительные игровые элементы

- `AGAS_RepGraphProjectile` - базовый физический снаряд
- `AEffectActor` - серверный overlap-актор, применяющий `GameplayEffect` и удаляющийся после применения

## Зависимости модуля

`Source/GAS_RepGraph/GAS_RepGraph.Build.cs`:

- `Core`
- `CoreUObject`
- `Engine`
- `InputCore`
- `EnhancedInput`
- `GameplayTags`
- `GameplayAbilities`
- `GameplayTasks`

## Запуск проекта

1. Откройте `GAS_RepGraph.uproject` в Unreal Engine 5.3+
2. Соберите C++ модуль (если требуется, через Visual Studio / Rider)
3. Проверьте, что в проекте активирован плагин `GameplayAbilities`
4. Запустите PIE с несколькими клиентами для проверки репликации и GAS-инициализации

## Текущее состояние и дальнейшее развитие

Проект уже пригоден как база для сетевой RPG/FPS с GAS и data-driven настройкой классов.  
Логичные следующие шаги:

- привязка input-тегов к способностям
- UI для атрибутов/кулдаунов/эффектов
- полноценная система урона/смерти/респавна
- расширение набора эффектов и способностей
- отдельная оптимизация сетевого слоя и репликации под боевые сценарии
