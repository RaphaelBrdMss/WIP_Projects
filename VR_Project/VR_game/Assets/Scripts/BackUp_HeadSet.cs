using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
public class BackUp_HeadSet : MonoBehaviour
{

    public InputActionReference horizontal;
    public InputActionReference vertical;
    [SerializeField] float headSpeedY = 0.5f;
    [SerializeField] float headSpeedX= 8f;
    public Transform camTranform;
    float move_Pitch;
    float move_Yaw;

    // Start is called before the first frame update
    void Start()
    {
        Cursor.lockState = CursorLockMode.Locked;
        vertical.action.performed += HandleVerticalChange;
        horizontal.action.performed += HandleHorizontalChange;
       
    }

     void HandleVerticalChange(InputAction.CallbackContext obj)
    {
        move_Pitch -= obj.ReadValue<float>();
        camTranform.localRotation = Quaternion.AngleAxis(move_Pitch , Vector3.right * headSpeedX);

    }

     void HandleHorizontalChange(InputAction.CallbackContext obj)
    {
         
        move_Yaw += obj.ReadValue<float>();
        camTranform.localRotation = Quaternion.AngleAxis(move_Yaw , Vector3.up * headSpeedY);
    }

    // Update is called once per frame
    void Update() 
    {
        

    }
}
