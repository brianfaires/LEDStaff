void UpdateChaser(Chaser* c) { // 200 bytes
  if(c->Loc < NUM_LEDS/3 && c->Loc % 2 == 1) {
    if(c->Increasing)
      delayRate--;
    else
      delayRate++;

    if(!c->Increasing) {
      RandWalk(&c->color.r, 10, maxBrightness, 3, 2);
      RandWalk(&c->color.g, 10, maxBrightness, 3, 2);
      RandWalk(&c->color.b, 10, maxBrightness, 3, 2);
    }
    
    if(c->Loc < NUM_LEDS/8) {
      if(c->Increasing)
        delayRate -= 2;
      else
        delayRate += 2;

      RandWalk(&c->color.r, 10, maxBrightness, delayRate, 2);
      RandWalk(&c->color.g, 10, maxBrightness, delayRate, 2);
      RandWalk(&c->color.b, 10, maxBrightness, delayRate, 2);
    }
  }
}
void DrawChaser(Chaser* c) {  
  leds(c->Loc, c->Loc + c->Width - 1) = c->color;
  
  if(c->Loc == c->Max - c->Width + 1) c->Increasing = false;
  else if(c->Loc == c->Min) c->Increasing = true;

  if(c->Increasing) c->Loc++;
  else c->Loc--;
}

