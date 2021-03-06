/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <EMotionFX/Source/EMotionFXConfig.h>
#include <EMotionFX/Source/EventHandler.h>
#include <EMotionFX/Source/AnimGraphTransitionCondition.h>


namespace EMotionFX
{
    // forward declarations
    class AnimGraphInstance;
    class AnimGraphMotionNode;

    /**
     *
     *
     *
     */
    class EMFX_API AnimGraphMotionCondition
        : public AnimGraphTransitionCondition
    {
    private:
        class EventHandler;

    public:
        AZ_RTTI(AnimGraphMotionCondition, "{0E2EDE4E-BDEE-4383-AB18-208CE7F7A784}", AnimGraphTransitionCondition)
        AZ_CLASS_ALLOCATOR_DECL

        enum TestFunction : AZ::u8
        {
            FUNCTION_EVENT                  = 0,
            FUNCTION_HASENDED               = 1,
            FUNCTION_HASREACHEDMAXNUMLOOPS  = 2,
            FUNCTION_PLAYTIME               = 3,
            FUNCTION_PLAYTIMELEFT           = 4,
            FUNCTION_ISMOTIONASSIGNED       = 5,
            FUNCTION_ISMOTIONNOTASSIGNED    = 6,
            FUNCTION_NONE                   = 7
        };

        // the unique data
        class EMFX_API UniqueData
            : public AnimGraphObjectData
        {
            EMFX_ANIMGRAPHOBJECTDATA_IMPLEMENT_LOADSAVE

        public:
            AZ_CLASS_ALLOCATOR_DECL

            UniqueData(AnimGraphObject* object, AnimGraphInstance* animGraphInstance, MotionInstance* motionInstance);
            ~UniqueData();

        public:
            MotionInstance*                             mMotionInstance;
            AnimGraphMotionCondition::EventHandler*     mEventHandler;
            bool                                        mTriggered;
        };

        AnimGraphMotionCondition();
        AnimGraphMotionCondition(AnimGraph* animGraph);
        ~AnimGraphMotionCondition();

        void Reinit() override;
        bool InitAfterLoading(AnimGraph* animGraph) override;

        void OnUpdateUniqueData(AnimGraphInstance* animGraphInstance) override;
        void OnRemoveNode(AnimGraph* animGraph, AnimGraphNode* nodeToRemove) override;

        void Reset(AnimGraphInstance* animGraphInstance) override;

        void GetSummary(AZStd::string* outResult) const override;
        void GetTooltip(AZStd::string* outResult) const override;
        const char* GetPaletteName() const override;

        bool TestCondition(AnimGraphInstance* animGraphInstance) const override;

        void SetTestFunction(TestFunction testFunction);
        TestFunction GetTestFunction() const;
        const char* GetTestFunctionString() const;

        void SetEventType(const AZStd::string& eventType);
        const AZStd::string& GetEventType() const;

        void SetEventParameter(const AZStd::string& eventParameter);
        const AZStd::string& GetEventParameter() const;

        void SetMotionNodeId(AnimGraphNodeId motionNodeId);
        AnimGraphNodeId GetMotionNodeId() const;
        AnimGraphNode* GetMotionNode() const;

        void SetNumLoops(AZ::u32 numLoops);
        AZ::u32 GetNumLoops() const;

        void SetPlayTime(float playTime);
        float GetPlayTime() const;

        void GetAttributeStringForAffectedNodeIds(const AZStd::unordered_map<AZ::u64, AZ::u64>& convertedIds, AZStd::string& attributesString) const override;

        static void Reflect(AZ::ReflectContext* context);

    private:
        // the motion instance event handler
        class EventHandler
            : public MotionInstanceEventHandler
        {
        public:
            AZ_CLASS_ALLOCATOR_DECL

            static EventHandler* Create(AnimGraphMotionCondition* condition, UniqueData* uniqueData);

            void OnEvent(const EventInfo& eventInfo) override;
            void OnHasLooped() override;
            void OnIsFrozenAtLastFrame() override;
            void OnDeleteMotionInstance() override;

        private:
            AnimGraphMotionCondition*  mCondition;
            UniqueData*                mUniqueData;

            EventHandler(AnimGraphMotionCondition* condition, UniqueData* uniqueData);
            ~EventHandler();
        };

        AZ::Crc32 GetNumLoopsVisibility() const;
        AZ::Crc32 GetPlayTimeVisibility() const;
        AZ::Crc32 GetEventPropertiesVisibility() const;

        static const char* s_functionMotionEvent;
        static const char* s_functionHasEnded;
        static const char* s_functionHasReachedMaxNumLoops;
        static const char* s_functionHasReachedPlayTime;
        static const char* s_functionHasLessThan;
        static const char* s_functionIsMotionAssigned;
        static const char* s_functionIsMotionNotAssigned;

        AZStd::string           m_eventType;
        AZStd::string           m_eventParameter;
        AZ::u64                 m_motionNodeId;
        AnimGraphMotionNode*    m_motionNode;
        AZ::u32                 m_numLoops;
        float                   m_playTime;
        TestFunction            m_testFunction;
    };
} // namespace EMotionFX