using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MouseLook : MonoBehaviour
{

    [SerializeField] float sensiX = 8f;
    [SerializeField] float sensiY = 100f;
    [SerializeField] Transform playerCam;
    [SerializeField] float xClamp = 85f ;
    float xRotation = 0f;

    float mouseX, mouseY;



    public void ReadInput(Vector2 M_input)
    {
        mouseX = M_input.x * sensiX;
        mouseY = M_input.y* sensiY;
    }
    // Start is called before the first frame update
    void Start()
    {
        
    }
    
    // Update is called once per frame
    void Update()
    {
        transform.Rotate(Vector3.up, mouseX * Time.deltaTime);

        xRotation -= mouseY;
        xRotation = Mathf.Clamp(xRotation, -xClamp, xClamp);
        Vector3 targetRotation = transform.eulerAngles;
        targetRotation.x = xRotation;
        playerCam.eulerAngles = targetRotation;

    }
}
