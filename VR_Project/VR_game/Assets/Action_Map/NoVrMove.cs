using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NoVrMove : MonoBehaviour
{

    Vector2 horizontalInput;
    [SerializeField] public CharacterController charControl;
    [SerializeField] public float speed=10f;

    // Start is called before the first frame update

    public void readInput(Vector2 H_input)
    {
        horizontalInput = H_input;
        Debug.Log(horizontalInput);
    }
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 horizontalSpeed =  (transform.right * horizontalInput.x + transform.forward * horizontalInput.y) *speed;
        charControl.Move(horizontalSpeed * Time.deltaTime);
    }
}
