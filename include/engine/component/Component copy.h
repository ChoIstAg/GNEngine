/*
 * GNEngine
 *
 * Copyright (c) 2024 GNEngine Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef COMPONENT_H
#define COMPONENT_H

/*
 * @brief 모든 컴포넌트의 기반이 되는 클래스임.
 * 순수한 가상 소멸자를 통해 다형적 파괴를 보장함.
*/
class Component {
public:
    virtual ~Component() = default;
};

#endif // COMPONENT_H
