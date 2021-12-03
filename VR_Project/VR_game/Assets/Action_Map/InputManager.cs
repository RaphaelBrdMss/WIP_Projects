using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InputManager : MonoBehaviour
{
    [SerializeField] public  NoVrMove moves;
    [SerializeField] MouseLook mouseLook;
    PlayersControl controls;
    PlayersControl.GroundMouvementActions groundMovement;
     
    Vector2 horizontalInput;
    Vector2 mousseInput;  

    private void Awake()
    {
        Debug.Log("controls init");
        controls = new PlayersControl();
        controls.Enable();
        groundMovement = controls.GroundMouvement;
        groundMovement.HorizontalMouvment.performed += ctx => horizontalInput = ctx.ReadValue<Vector2>();
        groundMovement.MouseX.performed += ctx => mousseInput.x = ctx.ReadValue<float>();
        groundMovement.MouseY.performed += ctx => mousseInput.y = ctx.ReadValue<float>();
        Debug.Log("Awake : " + horizontalInput);
        
    }

    // Update is called once per frame
    private void onDestroy()
    {
        controls.Disable();
    }

    private void Update()
    {
        Debug.Log("Update : " + horizontalInput);
        moves.readInput(horizontalInput);
        mouseLook.ReadInput(mousseInput);
    }
}
