#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UObject/ConstructorHelpers.h"

#include "physics/Kinematics.hpp"
#include "vehicles/car/api/CarApiBase.hpp"
#include "common/AirSimSettings.hpp"
#include "AirBlueprintLib.h"
#include "api/VehicleSimApiBase.hpp"
#include "common/common_utils/Signal.hpp"
#include "common/common_utils/UniqueValueMap.hpp"
#include "PawnEvents.h"
#include "PIPCamera.h"

#include "CarPawn.generated.h"

class UPhysicalMaterial;
class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;
class UAudioComponent;

UCLASS(config = Game)
class ACarPawn : public AWheeledVehicle
{
    GENERATED_BODY()

public:
    ACarPawn();

    virtual void BeginPlay() override;
    virtual void Tick(float Delta) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
                           FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

    //interface
    void initializeForBeginPlay(bool engine_sound);
    const common_utils::UniqueValueMap<std::string, APIPCamera*> getCameras() const;
    PawnEvents* getPawnEvents()
    {
        return &pawn_events_;
    }
    UWheeledVehicleMovementComponent* getVehicleMovementComponent() const;
    const msr::airlib::CarApiBase::CarControls& getKeyBoardControls() const
    {
        return keyboard_controls_;
    }
   
  

private:
    void updateHUDStrings();
    void setupVehicleMovementComponent();
    void updateInCarHUD();
    void updatePhysicsMaterial();

    void setupInputBindings();
    void onMoveForward(float Val);
    void onMoveRight(float Val);
    void onHandbrakePressed();
    void onHandbrakeReleased();
    void onFootBrake(float Val);
    void onReversePressed();
    void onReverseReleased();

    //ThrustMaster
    void onGasPedalInput(float Val);
    void onFootbrakeInput(float Val);
    void onSteeringWheelInput(float Val);
    void onClutchPedalInput(float Val);

    

private:
    typedef msr::airlib::AirSimSettings AirSimSettings;

    UClass* pip_camera_class_;

    PawnEvents pawn_events_;

    bool is_low_friction_;
    UPhysicalMaterial* slippery_mat_;
    UPhysicalMaterial* non_slippery_mat_;

    UPROPERTY()
    USceneComponent* camera_front_center_base_;
    UPROPERTY()
    USceneComponent* camera_front_left_base_;
    UPROPERTY()
    USceneComponent* camera_front_right_base_;
    UPROPERTY()
    USceneComponent* camera_driver_base_;
    UPROPERTY()
    USceneComponent* camera_back_center_base_;

    UPROPERTY()
    APIPCamera* camera_front_center_;
    UPROPERTY()
    APIPCamera* camera_front_left_;
    UPROPERTY()
    APIPCamera* camera_front_right_;
  /*  UPROPERTY()
    APIPCamera* camera_driver_;*/
    UPROPERTY()
    APIPCamera* camera_back_center_;

    UTextRenderComponent* speed_text_render_;
    UTextRenderComponent* gear_text_render_;
    UAudioComponent* engine_sound_audio_;

    msr::airlib::CarApiBase::CarControls keyboard_controls_;

    FText last_speed_;
    FText last_gear_;
    FColor last_gear_display_color_;
    FColor last_gear_display_reverse_color_;
public:
    UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
    float steering_input1;

    UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
        APIPCamera* camera_driver_;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        UForceFeedbackEffect* FSteering_Feedback;

    void forceFeedbackControlSetup();
};

//helper functions
float mapRangeClamped(float inVal, float minInRange, float maxInRange, float minOutRange, float maxOutRange);